<?php

/*

RSS feed load

*/
error_reporting(E_ALL);
ini_set('display_startup_errors',1);
ini_set('display_errors',1);
error_reporting(-1);



//header("Content-Type:text/plain; charset:utf-8");  //issue xml type, & causing error, left it at plain
require_once('../../../../wp-config.php');
require_once('../../../../wp-includes/wp-db.php');
require_once('../../../../wp-includes/pluggable.php');
require_once '../include/feedcommon102.php';

session_start();
ob_implicit_flush(true);
ob_end_flush();
global $wpdb;
$wpdb->show_errors();

$id = isset($_GET['id']) ? $_GET['id'] : '';
$id_from = isset($_GET['id_from']) ? $_GET['id_from'] : '';
$id_to = isset($_GET['id_to']) ? $_GET['id_to'] : '';
$custom = isset($_GET['custom']) ? $_GET['custom'] : '';
$debug = isset($_GET['debug']) ? $_GET['debug'] : '';
$error = isset($_GET['error']) ? $_GET['error'] : '';

date_default_timezone_set('UTC');

$start_time = date("Y-m-d H:i:s");
echo "start: " . $start_time . "<br>";
echo "debug: " . $debug . "<br>";
$start_time = new DateTime($start_time);
function get_headers_from_curl_response($response)
{
    $headers = array();

    $header_text = substr($response, 0, strpos($response, "\r\n\r\n"));

	//echo "<BR>";
	//echo $header_text;
	//echo "<BR>";
	
    foreach (explode("\r\n", $header_text) as $i => $line)
        if ($i === 0)
            $headers['http_code'] = $line;
        else
        {
            list ($key, $value) = explode(': ', $line);

            $headers[$key] = $value;
        }

    return $headers;
}


$table = "p9_categories";
// get list of feeds
if (!empty($id)) { 
	$query	= "select id, category, sub_category, feed_name, url, logo, header_required, timestamp_curl, timestamp_head FROM p9_categories where active = 'Y' and id = '$id' order by id asc";
} else {
	$query	= "select id, category, sub_category, feed_name, url, logo, header_required, timestamp_curl, timestamp_head FROM p9_user_feeds where active = 'Y' order by id asc";

}

//echo $query;


$result = $wpdb->get_results($query) ;
$curl = curl_init();

if (!empty($result)) {
	
	foreach( $result as $row1 ) {
	
		$now = date("Y-m-d H:i:s");
		$feed_start_time = new DateTime($now);
		
		$feed_id			= $row1->id;
		$feed_category 		= $row1->category;
		$feed_sub_category 	= $row1->sub_category;
		$feed_name 			= $row1->feed_name;
		$feed_url 			= $row1->url;
		$feed_logo 			= $row1->logo;
		$header_required 	= $row1->header_required;
		$timestamp_curl 	= $row1->timestamp_curl;
		$timestamp_head 	= $row1->timestamp_head;
		
		//echo str_repeat(' ',1024*64); //trying to get buffer to print / flush
		echo $feed_id . " ";
		echo $feed_name . " ";
		echo $feed_url . " ";
		echo $header_required . " ";
		
		// check last modified
		
		if (($header_required == "N") || ($header_required == "")) {
		
			//echo "getting filetime" . "<BR>";
			curl_setopt($curl, CURLOPT_URL, $feed_url); 
			curl_setopt($curl, CURLOPT_NOBODY, TRUE); /* don't retrieve body contents */
			curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE); /* follow redirects */
			curl_setopt($curl, CURLOPT_HEADER, FALSE); /* retrieve last modification time */
			curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
			curl_setopt($curl, CURLOPT_FILETIME, TRUE); /* get timestamp */
			
			$res = curl_exec($curl);
			$timestamp = curl_getinfo($curl, CURLINFO_FILETIME);
			//echo "timestamp: " . $timestamp;
				
			if ($timestamp > 0) {
				
				if ($timestamp != $timestamp_curl) {
				
					echo " Y ";
					get_feed($feed_url,$feed_name,$feed_id,$feed_logo,$curl,$wpdb,$table,$debug);
					$now = date("Y-m-d H:i:s");
					$wpdb->update($table, array('last_update'=>$now,'header_required'=>'N', 'timestamp_curl'=>$timestamp), array('id'=>$feed_id));
					
				
				}
				else
				{
				
					echo " N ";
					$now = date("Y-m-d H:i:s");
					$wpdb->update($table, array('last_update'=>$now), array('id'=>$feed_id));
					//continue;
				
				}
				
						
			} 
			else
			{
			
				echo " Y ";
				get_feed($feed_url,$feed_name,$feed_id,$feed_logo,$curl,$wpdb,$table,$debug);
					$wpdb->update($table, array('header_required'=>'Y', 'timestamp_head'=>$timestamp), array('id'=>$feed_id));
				
				
			}
		}
		else
		{
			
				echo " Y ";
				
				get_feed($feed_url,$feed_name,$feed_id,$feed_logo,$curl,$wpdb,$table,$debug);
				$wpdb->update($table, array('header_required'=>'Y', 'timestamp_head'=>$timestamp), array('id'=>$feed_id));
				
				
				
		}
			
		$feed_end_time 	= date("Y-m-d H:i:s");
		$feed_total_time	= $feed_start_time->diff(new DateTime($feed_end_time));
		echo $feed_total_time->format("%H:%I:%S") . " / ";
		
		$elapsed_time	= $start_time->diff(new DateTime($feed_end_time));
		echo $elapsed_time->format("%H:%I:%S") . "<br>";
		
	
	} // for each category
} // end if empty  
curl_close($curl);


$end_time 	= date("Y-m-d H:i:s");
$total_time	= $start_time->diff(new DateTime($end_time));
echo "time elapsed: " . $total_time->format("%H:%I:%S") . "<br>";
 
 
// now load main reporting table for users
echo "<br><B>Loading data to main table</B><br>";
$now = date("Y-m-d H:i:s");
//$query 	= "SELECT deviceid, username FROM `p9_devices_rss` where active ='Y' group by deviceid, username";

$query 	= "SELECT distinct username FROM `p9_devices_rss` a , wp_users b where a.active ='Y' and a.username = b.user_login and a.deviceid = 'default'";

$rdevices = $wpdb->get_results($query) ;

//print_r($rdevices);


if ($rdevices) {
	foreach( $rdevices as $rdevice ) {
	
		$user_array['u_deviceid']		= 'default'; 
		$user_array['u_userid']			= $rdevice->username;
		//load_rss ($user_array, $wpdb, $debug);
		
		//echo $user_array['u_userid'];
		
		//$wpdb->query('START TRANSACTION');
		$wpdb->query( $wpdb->prepare( 
		"DELETE FROM p9_device_content
		WHERE deviceid=(%s) and username=(%s) and source = 'RSS'", 
		$user_array['u_deviceid'],$user_array['u_userid']
		) );
		//$wpdb->query('COMMIT');
		 			
		//$wpdb->query('START TRANSACTION');
		$wpdb->query( $wpdb->prepare( 
		"
		INSERT IGNORE INTO p9_device_content
			(deviceid, username, pubdate, source, heading, body, item_title, item_content, format, item_url, logo, feed_id, category, sub_category, s3key, dt_created)
		SELECT 'default', b.username, a.item_date, 'RSS', a.feed_name, substr(concat(a.item_title, ' - ', a.item_content),1,250) , a.item_title, a.item_content, 0, a.item_url, a.logo, a.feed_id, c.category, c.sub_category, a.s3key, now()
		FROM 
			p9_rss_content a use index(idx_date),
			p9_devices_rss b,
			p9_categories c
			
		where a.feed_id = b.feed_id 
			and b.deviceid = '%s' 
			and b.username='%s' 
			and b.active = 'Y'
			and a.feed_id = c.id
			and a.item_date <= now()
		ORDER BY item_date DESC  
		LIMIT 200
		", 
		$user_array['u_deviceid'],$user_array['u_userid']
		) );
		//$wpdb->query('COMMIT');
		
	}
}

$end_time 	= date("Y-m-d H:i:s");
$total_time	= $start_time->diff(new DateTime($end_time));
echo "time elapsed: " . $total_time->format("%H:%I:%S") . "<br>";


// now load main reporting table for users
echo "<br><B>Loading data to main table for custom</B><br>";
$now = date("Y-m-d H:i:s");
$query 	= "SELECT distinct username FROM `p9_user_feeds` where active ='Y' ";
$rdevices = $wpdb->get_results($query) ;

if ($rdevices) {
	foreach( $rdevices as $rdevice ) {
	
		$user_array['u_deviceid']		= "default"; 
		$user_array['u_userid']			= $rdevice->username;
		//load_rss_custom ($user_array, $wpdb, $debug);
		
		//$wpdb->query('START TRANSACTION');
		$wpdb->query( $wpdb->prepare( 
		"DELETE FROM p9_device_content
		WHERE deviceid=(%s) and username=(%s) and source = 'RSSC'", 
		$user_array['u_deviceid'],$user_array['u_userid']
		) );
		//$wpdb->query('COMMIT');
		 			
		//$wpdb->query('START TRANSACTION');
		$wpdb->query( $wpdb->prepare( 
		"
		INSERT IGNORE INTO p9_device_content
			(deviceid, username, pubdate, source, heading, body, item_title, item_content, format, item_url, logo, feed_id, category, sub_category, s3key, dt_created)
		SELECT 'default', b.username, a.item_date, 'RSSC', a.feed_name, substr(concat(a.item_title, ' - ', a.item_content),1,250), a.item_title, a.item_content ,0, a.item_url, a.logo, a.feed_id, c.category, c.sub_category, a.s3key, now()
		FROM 
			p9_rss_content a use index(idx_date),
			p9_user_feeds b,
			p9_categories c
			
		where a.feed_id = b.feed_id 
			and b.deviceid = '%s' 
			and b.username='%s' 
			and b.active = 'Y'
			and a.feed_id = c.id
			and a.item_date <= now()
		ORDER BY item_date DESC  
		LIMIT 200
		", 
		$user_array['u_userid']
		) );
		//$wpdb->query('COMMIT');
		
	}
}


echo "<br>";


$end_time 	= date("Y-m-d H:i:s");
$total_time	= $start_time->diff(new DateTime($end_time));
	
echo "total time: " . $total_time->format("%H:%I:%S") . "<br>";
 

function get_feed($feed_url,$feed_name,$feed_id,$feed_logo,$curl,$wpdb,$table,$debug)
{

		//echo "<br>get feed: " . $feed_url;
	
	
	       
		curl_setopt($curl, CURLOPT_URL, $feed_url); 
		curl_setopt($curl, CURLOPT_NOBODY, FALSE); /* don't retrieve body contents */
		curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE); /* follow redirects */
		curl_setopt($curl, CURLOPT_HEADER, FALSE); /* retrieve last modification time */
		curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($curl, CURLOPT_ENCODING, TRUE);
		//curl_setopt($curl, CURLOPT_FILETIME, TRUE); /* get timestamp */
			
			
		$data = curl_exec($curl);
		//curl_close($curl);
        
		//print_r($data);
		
		//$xml = simplexml_load_string($data);
		//echo $data;
		
		libxml_use_internal_errors(true);
		//$RSS_DOC = simpleXML_load_file($feed_url);
		$RSS_DOC = simplexml_load_string($data);
	    
		
		if (!$RSS_DOC) {
			echo "Failed loading XML\n";
			foreach(libxml_get_errors() as $error) {
				echo "\t", $error->message;
			}
			$now = date("Y-m-d H:i:s");
			$status = "Error";
			$wpdb->update($table, array('last_status'=>$status), array('id'=>$feed_id));
				
			//echo $wpdb->last_query;
			
		}
		//print_r($RSS_DOC);
		else 
		{
			if (isset($RSS_DOC->channel)) {
				
					if ($debug == "Y") { 
						echo "rss";
					}
					
					/* Get title, link, managing editor, and copyright from the document  */
					$rss_title = $RSS_DOC->channel->title;
					$rss_link = $RSS_DOC->channel->link;
					$rss_editor = $RSS_DOC->channel->managingEditor;
					$rss_copyright = $RSS_DOC->channel->copyright;
					$rss_date = $RSS_DOC->channel->pubDate;
					
					
					$fetch_date = gmdate('Y-m-d H:i:s');
					//Loop through each item in the RSS document
					$cnt=0;
					foreach($RSS_DOC->channel->item as $RSSitem)
					
					{
						
						if($cnt == 9 ) {
							break;
						}  
						$cnt++;
						
						
						$item_id 		= md5($RSSitem->title);
						$rss_pubdate 	= $RSSitem->pubDate;
						if (empty($rss_pubdate)){ // rotoworld not using pubDate
						//	$rss_pubdate = $RSSitem->a10:updated; not sure how to get a10:updated
							$rss_pubdate = $fetch_date;
							//echo "date: ", $rss_pubdate;
						}
					
						$item_title 	= $RSSitem->title;
						$item_date  	= date("Y-m-d H:i:s", strtotime($rss_pubdate));
						$item_url		= $RSSitem->link;
						$item_content	= $RSSitem->description;
				
						if ($debug == "Y") { 
							echo "Processing item '" , $item_id , "' on " , $fetch_date 	, "<br/>";
							echo "  " . $item_title, " - ";
							echo "  " . $item_date, "<br/>";
							echo "  " . $item_url, "<br/>";
							//echo "I1  " . $item_content, "<br/>";
						}
						
						$item_content = preg_replace('/<[^>]*>/', '', $item_content);
						
					
						// prepare for db insert
						$item_title = html_entity_decode($item_title);
						$item_content = html_entity_decode($item_content);
						$item_title	= preg_replace('/[^A-Za-z0-9_ :@$^*!?\'\#\[\]\.\,\(\)%&-]/', '', $item_title);
						// $item_content = preg_replace('/[^A-Za-z0-9_ :@$^*\#\[\]\.\,\(\)%&-]/', '', $item_content);
						$item_content = preg_replace('/[^A-Za-z0-9_ :@$^*!?\'\#\[\]\.\,\(\)%&-]/', '', $item_content);
						//remove extra spaces
						$item_title = trim( preg_replace( '/\s+/', ' ', $item_title ) );  
						$item_content = trim( preg_replace( '/\s+/', ' ', $item_content ) );  
						
					//	echo "I3  " . $item_content, "<br/>";
						//$pub_dt = gmdate('Y-m-d H:i:s', strtotime($pub_dt));
						
						//$item_content = preg_replace('/&#039/,'\'',$item_content);		
						$item_content = str_replace("&#039", "'", $item_content);
						$item_content = str_replace("&#39", "'", $item_content);
					
						if ($debug == "Y") { 
					
							echo $item_content, "<br/>";
							//$item_content = html_entity_decode($item_content, ENT_QUOTES);
					
							echo "<font color=green>Inserting new item..</font><br/>";
						}
						
						//$wpdb->query('START TRANSACTION');
						
						$wpdb->query( $wpdb->prepare( 
						"
								INSERT IGNORE INTO p9_rss_content
								(feed_id, item_id, feed_name, feed_url, item_content, item_title, item_date, item_url, logo, fetch_date) 
								VALUES ( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s )
								
						", 
						$feed_id, $item_id,$feed_name,$feed_url,$item_content,$item_title,$item_date,$item_url,$feed_logo,$fetch_date
							
						) );
					
					
						//$wpdb->query('COMMIT');
						
						
						
						if ($debug == "Y") { echo "<br/>"; }
										
			
				} // for each rss item
				
				$now = date("Y-m-d H:i:s");
				$status = "Success";
				$wpdb->update($table, array('last_update'=>$now, 'last_status'=>$status), array('id'=>$feed_id));
				
								
				//echo $wpdb->last_query;
				
			}
			else if (isset($RSS_DOC->entry)) {
			
				if ($debug == "Y") { echo "atom"; }
				
				/* Get title, link, managing editor, and copyright from the document  */
					$rss_title = $RSS_DOC->title;
					$rss_link = $RSS_DOC->id;
					//$rss_editor = $RSS_DOC->channel->managingEditor;
					$rss_copyright = $RSS_DOC->rights;
					$rss_date = $RSS_DOC->updated;
					
					
					$fetch_date = gmdate('Y-m-d H:i:s');
					//Loop through each item in the RSS document
					$cnt=0;
					foreach($RSS_DOC->entry as $RSSitem)
					
						
					{
						
						if ($debug == "Y") { 
							print_r($RSSitem);
							echo "<br>";
						}
						
						if($cnt == 9 ) {
							break;
						}  
						$cnt++;
						
						
						$item_id 		= md5($RSSitem->title);
						$rss_pubdate 	= $RSSitem->published ;
						if (empty($rss_pubdate)){ // rotoworld not using pubDate
							$rss_pubdate = $RSSitem->updated;  //not sure how to get a10:updated
							//$rss_pubdate = $fetch_date;
							//echo "date: ", $rss_pubdate;
						}
					
						$urlAtt = $RSSitem->link->attributes();
						$item_url = $urlAtt['href'];
						
						$item_title 	= $RSSitem->title;
						$item_date  	= date("Y-m-d H:i:s", strtotime($rss_pubdate));
						$item_content	= $RSSitem->summary;
				
						if ($debug == "Y") { 
							echo "Processing item '" , $item_id , "' on " , $fetch_date 	, "<br/>";
							echo "  " . $item_title, " - ";
							echo "  " . $item_date, "<br/>";
							echo "  " . $item_url, "<br/>";
							//echo "I1  " . $item_content, "<br/>";
						}
						$item_content = preg_replace('/<[^>]*>/', '', $item_content);
						
					
						// prepare for db insert
						$item_title = html_entity_decode($item_title);
						$item_content = html_entity_decode($item_content);
						$item_title	= preg_replace('/[^A-Za-z0-9_ :@$^*!?\'\#\[\]\.\,\(\)%&-]/', '', $item_title);
						// $item_content = preg_replace('/[^A-Za-z0-9_ :@$^*\#\[\]\.\,\(\)%&-]/', '', $item_content);
						$item_content = preg_replace('/[^A-Za-z0-9_ :@$^*!?\'\#\[\]\.\,\(\)%&-]/', '', $item_content);
						//remove extra spaces
						$item_title = trim( preg_replace( '/\s+/', ' ', $item_title ) );  
						$item_content = trim( preg_replace( '/\s+/', ' ', $item_content ) );  
						
					//	echo "I3  " . $item_content, "<br/>";
						//$pub_dt = gmdate('Y-m-d H:i:s', strtotime($pub_dt));
						
						//$item_content = preg_replace('/&#039/,'\'',$item_content);		
						$item_content = str_replace("&#039", "'", $item_content);
						$item_content = str_replace("&#39", "'", $item_content);
						if ($debug == "Y") { 
							echo $item_content, "<br/>";
							//$item_content = html_entity_decode($item_content, ENT_QUOTES);
						
							echo "<font color=green>Inserting new item..</font><br/>";
						}
						//$wpdb->query('START TRANSACTION');
						
						$wpdb->query( $wpdb->prepare( 
						"
								INSERT IGNORE INTO p9_rss_content
								(feed_id, item_id, feed_name, feed_url, item_content, item_title, item_date, item_url, logo, fetch_date) 
								VALUES ( %s, %s, %s, %s, %s, %s, %s, %s, %s, %s )
								
						", 
						$feed_id, $item_id,$feed_name,$feed_url,$item_content,$item_title,$item_date,$item_url,$feed_logo,$fetch_date
							
						) );
					
					
						
						
						if ($debug == "Y") { echo "<br/>"; }
						
			
				} // for each atom item
				
				//$wpdb->query('COMMIT');
						
				$now = date("Y-m-d H:i:s");
				$status = "Success";
				$wpdb->update($table, array('last_update'=>$now, 'last_status'=>$status), array('id'=>$feed_id));
				
				//echo $wpdb->last_query;
						
				
			}
			else {
			
				echo "none";
				$now = date("Y-m-d H:i:s");
				$status = "Error";
				$wpdb->update($table, array('last_status'=>$status), array('id'=>$feed_id));
				
				//echo $wpdb->last_query;
				
			} // end if feed type rss/atom/other
		} // end if rss doc load error	
	

}
?>