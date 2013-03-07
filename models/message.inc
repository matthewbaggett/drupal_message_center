<?php 
class message extends versioned_active_record{
	protected $_table = "message_centre_messages";
	protected $_cfg_deleteable	= true;
	protected $_cfg_orderable	= false;
	
	public $message_body = '';
	public $message_heading = '';
	
	private function message_heading_slug(){
		return $this->_slugify(substr($this->message_heading, 0, 30));
	}
	
	public function get_slug(){
		return "{$this->mid}-{$this->message_heading_slug()}";
	}
}