<?php
// outlook mail api functions for feed*.php



// ----------------------------------------------------------------------------------------------------------------------------
// get users email from outlook     -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
function get_outlook_email ($user_array, $wpdb, $debug) {

	// for use with email api
	if ($debug == 'Y') {echo "here 123 \n";}

	//// Define API Client
	//$LiveApi = new \RayService\RayServiceLive(TickerApp::$apiConfigs);
    $LiveApi = new \RayService\RayServiceLive(TickerApp::$apiConfigs);
    if ($debug == 'Y') {echo "here live api \n";}
	//
	//// Set User [username, deviceid]
	//$device 			= 	$_SESSION['device'];
	//$current_user		=	wp_get_current_user();
	//$userid			=	$current_user->user_login;
	$userid 			= $user_array['u_userid'];
	$device 			= $user_array['u_deviceid']; 
	TickerApp::setUser($userid, $device);
	echo "body:";

	//// Get Database Token
	$databaseToken = TickerApp::updateAccessToken();
	
	
	//echo $databaseToken['body'];
		
	if ($debug == 'Y') { print_r($databaseToken);}

	
	
    //
	//// Get Mails
	if ($databaseToken):
		$emails = TickerApp::getMessages();
	endif;



}





?>