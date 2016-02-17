<?php

// ----------------------------------------------------------------------------------------------------------------------------
//  get gmail from users account ----------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------
function get_gmail($user_array, $wpdb, $debug) { 

			
				// wtf(get_access_token());
				//$user_info = get_user_info();
				
				if ($debug == 'Y') { 
					echo "user array: \n";
					print_r($user_array);
				}
				
				$device				= $user_array['u_deviceid'];
				$username			= $user_array['u_userid'];
				$gmail_id           = $user_array['id'];
				$access_token		= $user_array['u_g_accesstoken'];
				$refresh_token		= $user_array['u_g_refreshtoken'];
				$expires 			= $user_array['u_g_expires'];
				$id2 				= $user_array['id2'];
				
				$fail = "N";
				
				//verify access token_get_all
				//$vf_token_result = @file_get_contents('https://www.googleapis.com/oauth2/v1/tokeninfo?access_token=' .  $refresh_token);
				$vf_token_result = @file_get_contents('https://www.googleapis.com/oauth2/v1/tokeninfo?access_token=' .  $access_token);
				//echo "<br>" . $access_token . "br>";
				$response = (array) json_decode( $vf_token_result );
				//print_r($response);
				if (isset($response['email'])) {
					$emailid = $response['email'];
				}
				if ($debug == 'Y') { 
					echo "validate token response: \n";
					print_r($response);
				}
				
				//if( $vf_token_result === FALSE ) {
				if( empty($response) ) {
						//get new token
					
					$client_id		= _GOOGLE_CLIENT_ID;
					$client_secret	= _GOOGLE_CLIENT_SECRET;
					$fields = array(
						'client_id'		=> ( $client_id ),
						'client_secret'	=> ( $client_secret ),
						'refresh_token'	=> ( $refresh_token ),
						'grant_type'	=> ( 'refresh_token' )
					);
		
					if ($debug == 'Y') { 
						echo "fields: \n";
						print_r($fields);
					}
					
					// $result = ch_post_response('https://accounts.google.com/o/oauth2/token', $fields);
					$result = ch_post_response('https://www.googleapis.com/oauth2/v3/token', $fields);
		
					// extracting access_token from response string
					$response 					= json_decode( $result );
					if ($debug == 'Y') { 
						echo "new token response: \n";
						print_r($response);
					}
					

					if (isset($response->access_token)) {
						$access_token = $response->access_token;
						$user_array['u_g_accesstoken'] = $access_token;
						$expires_in = time() + $response->expires_in;
						$user_array['u_g_expires'] = $expires_in;
					}
					
					if ($debug == 'Y') { 
						echo "refresh_token: ", $refresh_token;
						echo "\n";
					}

                    if(!isset($response->access_token) || $response->access_token == ''){
						echo "error \n";
						$fail = "Y";
                        saveStatusToDb(false,$username,'GMAIL');
                    }
					else 
					{
						// brw 20151009 - save tokens to p9_personal_accounts, not p9_devices
						$wpdb->query( $wpdb->prepare( 
										"
											UPDATE p9_personal_accounts SET token = %s,expires = %s WHERE deviceid = %s and username = %s and source = 'GMAIL' and id2 = %s
																	
										", 
										$access_token, date('Y-m-d H:i:s', $expires_in), $device, $username, $id2 
									
						) );
						
						//UPDATE p9_devices SET g_accesstoken = %s,g_expires = %s WHERE deviceid = %s
						if ($debug == 'Y') { 
							echo $wpdb->last_query; 
						}	
					}
				}
				
				if ($debug == 'Y') { 
					echo "Fail: " . $fail . "\n";
				}
				
				if ($fail != "Y") {
				
					$users_req_result	= file_get_contents('https://www.googleapis.com/oauth2/v2/userinfo?access_token=' . $access_token);
					//$users_req_result	= file_get_contents('https://www.googleapis.com/oauth2/v2/userinfo?access_token=' . $refresh_token);
					$users_info 		= (array) json_decode( $users_req_result );
					if ($debug == 'Y') { 
						echo "user info: \n";
						print_r($users_info);
					}
					$emailid = $users_info['email'];
					$url = 'https://www.googleapis.com/gmail/v1/users/' .$emailid. '/messages?maxResults=20';
					$users_req_result = ch_response( $url, $access_token );
					$messages = (array) json_decode( $users_req_result );
					if ($debug == 'Y') { 
						echo "messages: \n";
						//print_r($messages);
					}
			
						
					if( $messages !== FALSE ) {
					
							// clear existing messages
							// O = old, V = valid
							$wpdb->query( $wpdb->prepare( 
							"
								UPDATE p9_email_g SET status = 'O' WHERE username = %s AND deviceid = %s and g_emailid = %s
							",
							$username,$device, $gmail_id
							) );
							
							
							foreach( $messages as $messageA ) {
								//$msg_details = get_message( $users_info['email'], $message->id );
								//echo "messageA: ";
								//print_r($messageA);
								if (is_array ($messageA)) {
									foreach( $messageA as $value ) {
										//print_r($value);
										$msg_id = $value->id;
							
										$url = 'https://www.googleapis.com/gmail/v1/users/' .$emailid. '/messages/' . $msg_id;
										$users_req_result = ch_response($url, $access_token );
								
										$msg_details = (array) json_decode( $users_req_result );
										
										if ($debug == 'Y') { 
											echo "id: " . $msg_details['id'];
											//print_r($msg_details);
										}
										
										if (isset($msg_details['id'])) {
											$good_message = format_message($emailid, $msg_details, $username, $device);
								
											if ($debug == 'Y') { 
												echo "good message: \n"; 
												print_r($good_message);
												
											}
									
											if ($good_message['inbox'] == 'Y') {
												$wpdb->query( $wpdb->prepare( 
													"
														INSERT IGNORE INTO p9_email_g
														(deviceid, username, pubDate, from_email, from_name, subject, body, read_flag, attachment, id, category, g_emailid, dt_created, status)
														VALUES ( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, 'V' )
														ON DUPLICATE KEY UPDATE read_flag=%s, status='V'
												
													", 
													$device,$username,$good_message['pubDate'],$good_message['from_email'],$good_message['from_name'],$good_message['subject'],$good_message['body'],$good_message['read_flag'],
													$good_message['attachment'],$good_message['id'],$good_message['category'],$good_message['g_emailid'],$good_message['dt_created'],
													$good_message['read_flag'],$good_message['category']
												) );
											}
										}
										//echo $wpdb->last_query; 
									}
								}
							}
						saveStatusToDb(true,$username,'GMAIL');
					} else {
						echo '<p>*No mails!</p>';
						saveStatusToDb(false,$username,'GMAIL');
					}
				} // end if not fail
}
		
function format_message( $userId, $message, $username, $device ) {
	 //wtf($message);
	 date_default_timezone_set('UTC');
	 
	 //echo "message:\n";
	 //print_r($message);
	 
	$now = date('Y-m-d H:i:s');
	$id = $message['id'];
	$threadId = $message['threadId'];
	// header attributes
	$from_email = get_msg_attr($message['payload']->headers, 'Authentication-Results');
	if( $from_email ) {
		$smtp_mail = str_replace('smtp.mail=', '', stristr($from_email, 'smtp.mail='));
		$temp = stristr($smtp_mail, ';', true);
		$from_email = ($temp === FALSE ? $smtp_mail : $temp);
	}

	$inbox      = get_inbox($message['labelIds']);
	$category = get_gm_category($message['labelIds']);
	$unread = get_unread($message['labelIds']);
	$from_name = get_msg_attr($message['payload']->headers, 'From');
	$pos = strrpos($from_name, "<");  
    if ($pos == true) {
        $from_name  = substr($from_name, 0, strrpos($from_name, "<"));  
        $from_name  = str_replace("\"", "", $from_name); // remove quotes
    }
    //echo "from2: ", $from_name , " \n";
    // remove any lingering <>
    $from_name  = str_replace("<", "", $from_name); // remove quotes
    $from_name  = str_replace(">", "", $from_name); // remove quotes
	
	$subject = get_msg_attr($message['payload']->headers, 'Subject');
	$body = $message['snippet'];
	
	$attachment = isset($message['payload']->parts) ? 'Y' : 'N';

	$pubDate = get_msg_attr($message['payload']->headers, 'Date');
	$pubDate = $pubDate ? date('Y-m-d H:i:s', strtotime($pubDate)) : $pubDate;
    
	// strip url links
	//$regex = "@(https?://([-\w\.]+[-\w])+(:\d+)?(/([\w/_\.#-]*(\?\S+)?[^\.\s])?)?)@";
	// 20150326 added =
	$regex = "@(https?://([-\w\.]+[-\w])+(:\d+)?(/([\w/_\.#=-]*(\?\S+)?[^\.\s])?)?)@";
	$body = preg_replace($regex, ' ', $body);
		
	// remove all tags
	$body 		= preg_replace('/<[^>]*>/', '', $body);
	$body 		= html_entity_decode($body);
	// remove spaces
	$body 		= trim( preg_replace( '/\s+/', ' ', $body ) );  
	// remove funky chars
	$body 		= preg_replace('/[^A-Za-z0-9_ :@$^*!?\#\[\]\.\,\(\)%&-]/', '', $body);
	$body 		= str_replace("&#39;", "'", $body);
	$body 		= str_replace("&#39", "'", $body);
	
	// remove all tags
	$subject 	= preg_replace('/<[^>]*>/', '', $subject);
	$subject 	= html_entity_decode($subject);
	// remove spaces
	$subject 	= trim( preg_replace( '/\s+/', ' ', $subject ) );  
	// remove funky chars
	$subject 	= preg_replace('/[^A-Za-z0-9_ :@$^*!?\#\[\]\.\,\(\)%&-]/', '', $subject);
	$subject 	= str_replace("&#39;", "'", $subject);
	$subject 	= str_replace("&#39", "'", $subject);
	
	$data = array(
		'deviceid' 		=> $device,
		'id'			=> $id,
		// 'threadId'		=> $threadId,
		'username'		=> $username,
		'from_email'	=> $from_email,
		'from_name'		=> $from_name,
		'category'		=> $category,
		'subject'		=> $subject,
		'body'			=> $body,
		'read_flag'		=> $unread,
		'attachment'	=> $attachment,
		'pubDate'		=> $pubDate,
		'g_emailid'		=> $userId,
		'dt_created'	=> $now,
        'inbox'         => $inbox
	);
	
	return $data;
}

function get_unread( $resource ) {
	$cat = 'R';
	foreach($resource as $i => $c) {
		if( strpos($c, 'UNREAD') !== FALSE ) {
			$cat = 'U';
		} 
	}
	return $cat;
}
function get_gm_category( $resource ) {
	$cat = 'Primary';
	foreach($resource as $i => $c) {
		if( strpos($c, 'SENT') !== FALSE ) {
			$cat = 'SENT';
		}  elseif( strpos($c, 'DRAFT') !== FALSE ) {
			$cat = 'DRAFT';
		}  elseif( strpos($c, 'CATEGORY_') !== FALSE ) {
			$cat = strtolower(str_replace('CATEGORY_', '', $c));
		}
	}
	return $cat;
}
function get_inbox( $resource ) {
    $inbox = 'N';
    foreach($resource as $i => $c) {
        if( strpos($c, 'INBOX') !== FALSE ) {
            $inbox = 'Y';
            break;
        }  
    }
    return $inbox;
}
function get_msg_attr( $resource, $attr ) {
	$value = '';
	if( is_array($resource) ) {
		foreach( $resource as $row ) {
			if( $row->name == $attr ) {
				$value = $row->value;	break;
			}
		}
		return $value;
	} else {
		return FALSE;
	}
}

function msg_in_db( $id ) {
	$CI =& get_instance();
	$response = $CI->db->from('p9_email_g')
						->where('id', $id)
						->count_all_results();

	return $response ? TRUE : FALSE;
}


function ch_response( $url,$access_token ) {
	error_reporting(E_ALL);
	// wtf($url);
	$curl = curl_init();

	$headr = array();
	$headr[] = 'Content-length: 0';
	$headr[] = 'Content-type: application/json';
	$headr[] = 'Authorization: OAuth '.$access_token;

	curl_setopt($curl, CURLOPT_URL, $url);
	curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
	curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_setopt($curl, CURLOPT_HTTPHEADER,$headr);
	curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
	$result = curl_exec($curl);
	// wtf($result);die('ch_response');

	curl_close($curl);

	return $result;
}

function ch_post_response( $url, $fields ) {
	// url-ify the data for the POST
	$fields_string = '';
	$fields_string = http_build_query($fields);
	// echo "{$fields_string}";

	// open connection
	$ch = curl_init();
		
	// set the url, number of POST vars, POST data
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_POST, 5);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $fields_string);
	// Set so curl_exec returns the result instead of outputting it.
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
	// to trust any ssl certificates
	curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
	// execute post
	$result = curl_exec($ch);

	// close connection
	curl_close($ch);

	return $result;
}

		
		
?>
