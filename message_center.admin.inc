<?php 
function message_center_edit_message($slug){
	// Pull the TID out
	$mid_bits = explode("-", $slug, 2);
	$mid = $mid_bits[0];

	// Get the Message
	$message = message::search()->where('mid', $mid)->execOne();

	// Handle the POST
	if(count($_POST) > 0){
		if(message_center_check_permission('edit messages')){
			$message->message_heading = $_POST['message_heading'];
			$message->message_body = $_POST['message_body'];
			$message->save();
			drupal_set_message("Saved changes to {$message->message_heading}");
		}else{
			drupal_set_message('Cannot save changes - You do not have the "edit messages" permission');
		}
		header("Location: {$_SERVER['HTTP_REFERER']}");
		exit;
	}

	// Heading
	$output = '<h3>' . t('Message "<em>@message_geading</em>"', array('@message_geading' => $message->message_heading)) . '</h3>';

	// Get the form to change the tranche heading & blurb
	$output.= "<form method=\"post\" action=\"\">";
	$output.= "  <fieldset>";
	$output.= "    <legend>";
	$output.= "      <div class=\"fieldset-legend\">Edit this message:</div>";
	$output.= "    </legend>";
	$output.= "    <div class=\"fieldset-wrapper\">";

	$output.= "      <div class=\"form-item form-type-textfield\">";
	$output.= "        <label>Message Heading</label>";
	$output.= "        <input class=\"form-text\" type=\"text\" name=\"message_heading\" value=\"{$message->message_heading}\"/>";
	$output.= "      </div>";

	$output.= "      <div class=\"form-item form-type-textarea resizable\">";
	$output.= "        <label>Message Body</label>";
	$output.= "        <div class=\"form-textarea-wrapper resizable textarea-processed resizable-textarea\">";
	$output.= "          <textarea class=\"form-textarea\" name=\"message_body\" cols=\"60\" rows=\"5\">{$message->message_body}</textarea>";
	$output.= "          <div class=\"grippie\"></div>";
	$output.= "        </div>";
	$output.= "      </div>";

	$output.= "    </div>";
	$output.= "  </fieldset>";
	$output.= "  <div class=\"form-actions\" id=\"edit-actions\">";
	$output.= "    <input type=\"submit\" id=\"edit-submit\" name=\"op\" value=\"Save Message\" class=\"form-submit\">";
	$output.= "  </div>";
	$output.= "</form>";

	if($_SERVER['HTTP_REFERER']){
		$output.= l(t("Back to Message Center"), $_SERVER['HTTP_REFERER']);
	}
	return $output;
}

function message_center_delete_message($slug){
	// Pull the TID out
	$mid_bits = explode("-", $slug, 2);
	$mid = $mid_bits[0];
	
	// Get the Message
	$message = message::search()->where('mid', $mid)->execOne();
	
	// Can't find the message? REJECT!
	if(!$message){
		drupal_set_message("No such message!","error");
		header("Location: {$_SERVER['HTTP_REFERER']}");
		exit;
	}
	
	// Check permissions
	if(!message_center_check_permission('delete messages')){
		drupal_set_message("You lack the permission to delete this message","error");
	}else{
		$message->delete();
	}
	header("Location: {$_SERVER['HTTP_REFERER']}");
	exit;
}

function message_center_add_message(){
	$message = new message();
	$message->message_heading = "New Message";
	$message->message_body = " ";
	$message->save();
	$message->reload();
	$mid = $message->get_id();
	drupal_set_message("Created new Message.");
	drupal_goto("admin/message_center/edit/{$message->get_slug()}");
}