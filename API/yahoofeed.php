<?php
// yahoo mail api functions for feed*.php


// ----------------------------------------------------------------------------------------------------------------------------
// get users email from outlook     -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
function get_yahoo_email ($user_array, $wpdb, $debug) {

	require_once('YahooApp.php');
	
	//// Set User [username, deviceid]
	$userid 			= $user_array['u_userid'];
	$device 			= $user_array['u_deviceid']; 
	YahooApp::setUser($userid, $device);
	
	
	//// Get Database Token
	$databaseToken = YahooApp::updateAccessToken();
    //
	//// Get Mails
	if ($databaseToken):
		$emails = YahooApp::getMessages();
	endif;

}




?>