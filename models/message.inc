<?php
class message extends versioned_active_record {
  protected $_table = "message_centre_messages";
  protected $_cfg_deleteable = true;
  protected $_cfg_orderable = false;

  public $message_body = '';
  public $message_heading = '';
  public $sticky = 'No';

  private function message_heading_slug() {
    return $this->_slugify(substr($this->message_heading, 0, 30));
  }

  public function get_slug() {
    return "{$this->mid}-{$this->message_heading_slug()}";
  }

	public function get_read_path(){
		return "message_centre/read/{$this->get_slug()}";
	}

	public function get_summary($limit = 80){
		return $this->_cut_paragraph($this->message_body, $limit);
	}

	private function _cut_paragraph ($paragraph, $limit = 80) {
		$paragraph = preg_replace('/\s+/', ' ', $paragraph);

		// Sanity check first.
		if($limit > strlen($paragraph) || $limit < 0){
			return $paragraph;
		}

		// Get the first space after the minimum length
		$pos = strpos ($paragraph, ' ', $limit);

		if (false !== $pos) {
			$result = substr ($paragraph, 0, $pos);
			$result = trim($result,'.,!;[])() ');
			return $result . "... ";
		}
		return $paragraph;
	}

  /**
   * Is this Message sticky?
   *
   * @return bool
   */
  public function is_sticky(){
    return $this->sticky=="Yes" ? true : false;
  }

  /**
   * Set this Message to sticky. Has Implicit save.
   * @return $this
   */
  public function set_sticky(){
    $this->sticky = "Yes";
    $this->save();
    return $this;
  }

  /**
   * Set this Message to not sticky. Has Implicit save.
   * @return $this
   */
  public function set_not_sticky(){
    $this->sticky = "No";
    $this->save();
    return $this;
  }
}
