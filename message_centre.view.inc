<?php

function message_centre_edit_message($slug){
	$message = message::get_by_slug($slug);

	$view             = new StdClass();
	$view->messages   = array($message);
	$view->read_more       = false;

	return message_centre_template_view(MESSAGE_CENTRE_ROOT_PATH . "/templates/message_centre.phtml", $view);
}