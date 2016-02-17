<?php
// twitter functions for feed*.php

// ----------------------------------------------------------------------------------------------------------------------------
// get users twitter timeline feed  -------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
function get_twitter_timeline ($user_array, $wpdb, $debug) {
	
	require_once 'twitteroauth.php';
	
	$tab = array();
	$u_userid = $user_array['u_userid'];
	$u_deviceid = $user_array['u_deviceid'];
	$u_id = $user_array['id'];
	$u_id2 = $user_array['id2'];
	
	if ($debug == 'Y') {echo "u_userid:", $u_userid , "\n";} 
	$twitterConnection = new TwitterOAuth(
	      // poundnine
      	 'xxxx',
         'xxxxxxxxx',
		$user_array['u_token'],       // Access token
		$user_array['u_secret']		    	// Access token secret	
	);
	

	$twitterData = $twitterConnection->get(
		'statuses/home_timeline',
		array(
			'count'           => 50,
			'exclude_replies' => false
			)
	);
	
	$z = 0;
	if ($debug == 'Y') {
		//print_r($twitterData);
	}

    // ===================== check success =====================
    if(isset($twitterData->errors)){
        if ($debug == 'Y') {
            echo "===================== [ Error {$twitterData->errors[0]->message} ] =====================";
        }
        saveStatusToDb(false,$u_userid,'TWITTER');
        return;
    }else{
        saveStatusToDb(true,$u_userid,'TWITTER');
    }
    // ===================== end check success =====================

	foreach($twitterData as $item){
	
		$id				= $item->id;
		$post 			= $item->text;
		$pub_dt 		= $item->created_at;
		$name 			= $item->user->name;
		$profile_image 	= $item->user->profile_image_url;
		$media_url		= "";
		
		if (isset($item->entities->media[0]->media_url)) {
			$media_url		= $item->entities->media[0]->media_url;
		}
		
		if ($debug == 'Y') {
			echo "\n name:", $name , "\n";
			echo "\n post:", $post , "\n";
			echo "\n profile_image:", $profile_image , "\n";
			echo "\n media_url:", $media_url , "\n";
			//print_r($item);
			
		} 
		
		// prepare for db insert
		$post = html_entity_decode($post);
		//remove extra spaces
		$post = trim( preg_replace( '/\s+/', ' ', $post ) );  
		//Un-quotes a quoted string
		// wordpress does not allow real_escape, use prepare instead
		//$post = mysqli_real_escape_string( $wpdb, stripslashes( $post ) );
		//$name = mysqli_real_escape_string( $wpdb, stripslashes( $name ) );	
		//remove bad characters
		//preg_replace('/[^a-zA-Z0-9_ %\[\]\.\(\)%&-]/s', '', $String);
		// to allow backslash >> \\\\
		$post = preg_replace('/[^A-Za-z0-9_ :@$^*!?\#\[\]\.\,\(\)%&-\\\\]/', '', $post);
		$pub_dt = gmdate('Y-m-d H:i:s', strtotime($pub_dt));
		
		$regex = "@(https?://([-\w\.]+[-\w])+(:\d+)?(/([\w/_\.#-]*(\?\S+)?[^\.\s])?)?)@";
			
		if ($debug == 'Y') {
			echo "\n name:", $name , "\n";
			//echo "\n post:", $post , "\n";
			echo "\n pos2:", preg_replace($regex, ' ', $post), "\n";
			//echo "\n ";
		} 
		
		$post = preg_replace($regex, ' ', $post);
		
		$wpdb->query( $wpdb->prepare( 
			"
				INSERT IGNORE INTO p9_messages
				(deviceid, userid, pubDate, user, post, seq_nbr, type, id, account, profile_image_url, tweet_image_url)
				VALUES ( %s, %s, %s, %s, %s, %d, %s, %s, %s, %s, %s )
			", 
		$u_deviceid, $u_userid,$pub_dt,$name,$post,$z,'T', $id, $u_id, $profile_image, $media_url
		
		) );
		
		$z = $z + 1;
		
	}
	
}

?>