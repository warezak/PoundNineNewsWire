<?php

/*

 brad.warezak 
 April 2015
 main process to initiate load of users yahoo mail, outlook mail, feedly, twitter, and google mail.
*/
error_reporting(E_ALL & ~E_DEPRECATED);
ini_set('display_startup_errors', 1);
ini_set('display_errors', 1);
error_reporting(-1);


session_start();   //Tell PHP to start the session
global $wpdb;

header("Content-Type:text/plain; charset:utf-8");  //issue xml type, & causing error, left it at plain
// wordpress files
require_once('../../../wp-config.php');
require_once('../../../wp-includes/wp-db.php');
require_once('../../../wp-includes/pluggable.php');

require_once 'include/feedcommon102.php';
require_once 'include/accounts.php';

function saveStatusToDb($isSuccess,$username,$source){
    $arr = array('last_success'=>'','last_status'=>'');

    if($isSuccess){
        $arr['last_success'] = date('Y-m-d H:i:s');
        $arr['last_status'] = 'Success';
    }else{
        $arr['last_status'] = 'Error';
    }
    global $wpdb;
    $wpdb->update('p9_personal_accounts',$arr, array('username'=>$username,'source'=>$source));
}


error_reporting(E_ALL);
//ini_set('display_startup_errors',1);
//ini_set('display_errors',1);
//error_reporting(-1);
$wpdb->show_errors();

$user_array = array();
$feed_list = "";
$feed_list_fs = "";

//outlook
require_once('include/RayServiceLive.php');
require_once('include/OutlookApp.php');
use \Ticker\TickerApp;

//feedly
require_once('p9/feedly/RayService/RayServiceFeedly.php');
require_once('p9/feedly/Ticker/FeedlyApp.php');
use \Feedly\FeedlyApp;
	
	
// check if source is device
$device 		= isset($_GET['d']) ? $_GET['d'] : '';
$debug 			= isset($_GET['debug']) ? $_GET['debug'] : '';
$for_user		= isset($_GET['user']) ? $_GET['user'] : '';
$load_twitter 	= isset($_GET['tw']) ? $_GET['tw'] : '';
$load_facebook 	= isset($_GET['fb']) ? $_GET['fb'] : '';
$load_linkedin 	= isset($_GET['ln']) ? $_GET['ln'] : '';
$load_feedly	= isset($_GET['fy']) ? $_GET['fy'] : '';
$load_ymail 	= isset($_GET['ym']) ? $_GET['ym'] : '';
$load_outlook 	= isset($_GET['om']) ? $_GET['om'] : '';
$load_gmail 	= isset($_GET['gm']) ? $_GET['gm'] : '';

// ----------------------------------------------------------------------------------------------------------------------------
// get list to load from   ---------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------

$network_list = "'abcd'";
if ($load_twitter 	== "Y") {$network_list = $network_list . ",'TWITTER'";}	
if ($load_facebook 	== "Y") {$network_list = $network_list . ",'FACEBOOK'";}
if ($load_linkedin 	== "Y") {$network_list = $network_list . ",'LINKEDIN'";}
if ($load_feedly	== "Y") {$network_list = $network_list . ",'FEEDLY'";}
if ($load_ymail 	== "Y") {$network_list = $network_list . ",'YMAIL'";}
if ($load_outlook 	== "Y") {$network_list = $network_list . ",'OMAIL'";}
if ($load_gmail 	== "Y") {$network_list = $network_list . ",'GMAIL'";}
if ($debug == 'Y') {echo "network_list: ", $network_list, "\n";}

$query 	= <<<EOD
select * from p9_personal_accounts where source in ($network_list) and active = 'Y'
order by source
EOD;

if (!empty($for_user)) {
$query 	= <<<EOD
select * from p9_personal_accounts where source in ($network_list) and active = 'Y' and username = '$for_user'
order by source
EOD;
}

$now = date("Y-m-d H:i:s");
echo "now start feed updates: 		", $now, "\n";

$result = $wpdb->get_results($query) ;
if ($result) {
	foreach( $result as $row ) {

		$user_array['u_deviceid'] 		= $row->deviceid;
		$user_array['u_userid']			= $row->username;
		$source							= $row->source;
		$id								= $row->id;
		$id2							= $row->id2;
		$user_array['id']               = $row->id;
		$user_array['id2']              = $row->id2;
		$user_array['u_token']			= $row->token;
		$user_array['u_refreshtoken']	= $row->refreshtoken;
		$user_array['u_secret']			= $row->secret;
		$user_array['u_expires']        = $row->expires;
		$handle			= $row->handle;
		$expires		= $row->expires;
		$last_update	= $row->last_update;
		
		// ----------------------------------------------------------------------------------------------------------------------------
		// get feed updates   ---------------------------------------------------------------------------------------------
		// ----------------------------------------------------------------------------------------------------------------------------

		if ($source == "TWITTER") {
			$now = date("Y-m-d H:i:s");
			echo "twitter start:        ", $now, "   for ", $user_array['u_userid']	, "-" , $user_array['u_deviceid'] ,  " - " , $user_array['id'] , " \n";
			require_once 'include/twitterfeed.php';
			// do not get twitter content for poundninenews when not clone id
			if (($user_array['id'] != "poundninenews") || (($user_array['id'] == "poundninenews") && ($user_array['u_userid'] == "clone"))) {
				get_twitter_timeline ($user_array, $wpdb, $debug);
				load_twitter ($user_array, $wpdb, 'N', $debug);
			}
			else 
			{
				copy_twitterclone($user_array['u_userid'], $wpdb, $debug);
				load_twitter ($user_array, $wpdb, 'Y', $debug);
			}
			
			$now = date("Y-m-d H:i:s");
			echo "twitter end:          ", $now, "\n";
		}
		
		if ($source == "YMAIL") {
			$now = date("Y-m-d H:i:s");
			echo "yahoo mail start:     ", $now, "   for ", $user_array['u_userid']	, "-" , $user_array['u_deviceid'] , " \n";
			require_once 'p9/yahoo/feed.php';
			get_yahoo_email ($user_array, $wpdb, $debug);
			load_ymail ($user_array, $wpdb, $debug); 
			$now = date("Y-m-d H:i:s");
			echo "yahoo mail end:       ", $now, "\n";
		} 
		
		if ($source == "GMAIL") {
			$now = date("Y-m-d H:i:s");
			echo "gmail start:         ", $now, "   for ", $user_array['u_userid']	, "-" , $user_array['u_deviceid'] ,  " - " , $user_array['id'] , " \n";
			$user_array['u_g_accesstoken']  = $row->token;
			$user_array['u_g_refreshtoken'] = $row->refreshtoken;
			$user_array['u_g_expires']		= $row->expires;
			$user_array['id2']				= $row->id2;
			require_once 'include/gmailfeed.php';
			get_gmail ($user_array, $wpdb, $debug);
			load_gmail ($user_array, $wpdb, $debug);
			$now = date("Y-m-d H:i:s");
			echo "gmail mail end:       ", $now, "\n";
		}
		if ($source == "OMAIL") {
			$now = date("Y-m-d H:i:s");
			echo "outlook start:        ", $now, "   for ", $user_array['u_userid']	, "-" , $user_array['u_deviceid'] ,  " - " , $user_array['id'] , " \n";
			get_outlook_email ($user_array, $wpdb, $debug);
			load_outlook ($user_array, $wpdb, $debug); 
			$now = date("Y-m-d H:i:s");
			echo "outlook end:          ", $now, "\n";
		}
		if ($source == "FEEDLY") {
			$now = date("Y-m-d H:i:s");
			echo "feedly start:         ", $now, "   for ", $user_array['u_userid']	, "-" , $user_array['u_deviceid'] ,  " - " , $user_array['id'] , " \n";
			get_feedly_feed ($user_array, $wpdb, $debug);
			load_feedly ($user_array, $wpdb, $debug);
			$now = date("Y-m-d H:i:s");
			echo "feedly end:           ", $now, "\n";
		}


	}
}


 
 
$now = date("Y-m-d H:i:s");
echo "done: 					", $now, "\n";

//end php page execution
// ----------------------------------------------------------------------------------------------------------------------------

function copy_twitterclone($username, $wpdb, $debug) {


		$wpdb->query( $wpdb->prepare( 
		"
		INSERT IGNORE INTO p9_messages (deviceid, userid, pubDate, user, post, dt_created, seq_nbr, type, id, account, profile_image_url, tweet_image_url) 
		SELECT 'default', %s, pubDate, user, post, dt_created, seq_nbr, type, id, account, profile_image_url, tweet_image_url
		from p9_messages 
		where deviceid = 'default' and userid = 'clone'
		order by pubDate desc limit 50
		",
		$username
		) );


}


			
// ----------------------------------------------------------------------------------------------------------------------------
// get users email from outlook     -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
function get_outlook_email ($user_array, $wpdb, $debug) {
    //// Define API Client
    //$LiveApi = new \RayService\RayServiceLive(TickerApp::$apiConfigs);
    $LiveApi = new \RayService\RayServiceLive(TickerApp::$apiConfigs);

    try {
        $userid 			= $user_array['u_userid'];
        $device 			= $user_array['u_deviceid'];
		$id2 				= $user_array['id2'];
		$id 				= $user_array['id'];
        TickerApp::setUser($userid, $device);
		TickerApp::setId2($id2, $id);
		       
        // Get Database Token
        $databaseToken = TickerApp::updateAccessToken();
        //echo $databaseToken['body'];
        if ($debug == 'Y') { print_r($databaseToken);}
        // Get Mails
        if ($databaseToken):
            $emails = TickerApp::getMessages();
        endif;

        if ($debug == 'Y') { echo "done 1";}
        saveStatusToDb(true,$userid,'OMAIL');
    }catch (Exception $e){
        saveStatusToDb(false,$userid,'OMAIL');
    }



}

// ----------------------------------------------------------------------------------------------------------------------------
// get users content from feedly    -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
function get_feedly_feed ($user_array, $wpdb, $debug) {


    FeedlyApp::getDatabaseConfigFromWordpress();
    //// Set User [username, deviceid]
    $userid 			= $user_array['u_userid'];
    $device 			= $user_array['u_deviceid'];
	$id2 				= $user_array['id2'];
	$id 				= $user_array['id'];   
    FeedlyApp::setUser($userid, $device);
	FeedlyApp::setId2($id2, $id);
	
    // Get Database Token
    $databaseToken = FeedlyApp::updateAccessToken();

    // Get Streams
    if ($databaseToken):
        $streams = FeedlyApp::getStreams();
    endif;

    if(!empty($streams) && isset($streams[0]['deviceid'])){
        saveStatusToDb(true,$userid,'FEEDLY');
    }else{
        saveStatusToDb(false,$userid,'FEEDLY');
    }

   //echo $userid.'<br />';
}

?>