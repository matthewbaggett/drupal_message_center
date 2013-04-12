<?php
function message_centre_edit_message($slug) {
  // Pull the TID out
  $mid_bits = explode("-", $slug, 2);
  $mid      = $mid_bits[0];

  // Get the Message
  $message = message::search()->where('mid', $mid)->execOne();

  // Handle the POST
  if (count($_POST) > 0) {
    if (message_centre_check_permission('edit messages')) {
      $message->message_heading = $_POST['message_heading'];
      $message->message_body    = $_POST['message_body'];
      $message->sticky          = $_POST['sticky'] == "Yes" ? "Yes" : "No";
      $message->save();
      drupal_set_message("Saved changes to {$message->message_heading}");
    } else {
      drupal_set_message('Cannot save changes - You do not have the "edit messages" permission');
    }

    if (isset($_REQUEST['dest'])) {
      header("Location: {$_REQUEST['dest']}");
    } else {
      header("Location: {$_SERVER['HTTP_REFERER']}");
    }
    exit;
  }

  // Add TinyMCE
  drupal_add_js(drupal_get_path('module', 'message_centre') . "/lib/tinymce/jscripts/tiny_mce/tiny_mce.js");
  drupal_add_js('
		tinyMCE.init({
			theme_advanced_toolbar_location : "top",
			mode : "textareas",
			theme : "simple",

		});
	', 'inline');
  drupal_add_css(drupal_get_path('module', 'message_centre') . '/tiny_mce_override.css');

  // Build the View
  $view          = new StdClass();
  $view->message = $message;

  // Decide how long ago the message was created. If it was created in the last 3 seconds, its a create, not edit.
  $view->create_or_edit = strtotime($message->created_date) > strtotime("3 seconds ago") ? 'create' : 'edit';
  drupal_set_title(ucfirst($view->create_or_edit) . " " . t("Message"));

  // Generate the edit admin panel
  $output = message_centre_template_view(MESSAGE_CENTRE_ROOT_PATH . "/templates/message.edit.phtml", $view);

  // If they have a HTTP_REFERER, offer a back button
  if ($_SERVER['HTTP_REFERER']) {
    $output .= l(t("Back to Message Centre"), $_SERVER['HTTP_REFERER']);
  }
  return $output;
}

function message_centre_delete_message($slug) {
  // Pull the TID out
  $mid_bits = explode("-", $slug, 2);
  $mid      = $mid_bits[0];

  // Get the Message
  $message = message::search()->where('mid', $mid)->execOne();

  // Can't find the message? REJECT!
  if (!$message) {
    drupal_set_message("No such message!", "error");
    header("Location: {$_SERVER['HTTP_REFERER']}");
    exit;
  }

  // Check permissions
  if (!message_centre_check_permission('delete messages')) {
    drupal_set_message("You lack the permission to delete this message", "error");
  } else {
    $message->delete();
  }
  header("Location: {$_SERVER['HTTP_REFERER']}");
  exit;
}

function message_centre_add_message() {
  $message                  = new message();
  $message->message_heading = "New Message";
  $message->message_body    = " ";
  $message->save();
  $message->reload();
  $mid = $message->get_id();
  drupal_set_message("Created new Message.");
  drupal_goto("admin/message_centre/edit/{$message->get_slug()}");
}
