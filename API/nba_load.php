<?php


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
global $wpdb;
$wpdb->show_errors();

$debug = isset($_GET['debug']) ? $_GET['debug'] : '';

date_default_timezone_set('UTC');

	
if ($debug == 'Y') {$wpdb->show_errors();}


// set team ids in array
$team_id = array(

	1700	=> 'Atlanta Hawks'
	,1701	=> 'Boston Celtics'
	,1702	=> 'Brooklyn Nets'
	,1703	=> 'Charlotte Hornets'
	,1704	=> 'Chicago Bulls'
	,1705	=> 'Cleveland Cavaliers'
	,1706	=> 'Dallas Mavericks'
	,1707	=> 'Denver Nuggets'
	,1708	=> 'Detroit Pistons'
	,1709	=> 'Golden State Warriors'
	,1710	=> 'Houston Rockets'
	,1711	=> 'Indiana Pacers'
	,1712	=> 'Los Angeles Clippers'
	,1713	=> 'Los Angeles Lakers'
	,1714	=> 'Memphis Grizzlies'
	,1715	=> 'Miami Heat'
	,1716	=> 'Milwaukee Bucks'
	,1717	=> 'Minnesota Timberwolves'
	,1718	=> 'New Orleans Pelicans'
	,1719	=> 'New York Knicks'
	,1720	=> 'Oklahoma City Thunder'
	,1721	=> 'Orlando Magic'
	,1722	=> 'Philadelphia 76ers'
	,1723	=> 'Phoenix Suns'
	,1724	=> 'Portland Trail Blazers'
	,1725	=> 'Sacramento Kings'
	,1726	=> 'San Antonio Spurs'
	,1727	=> 'Toronto Raptors'
	,1728	=> 'Utah Jazz'
	,1729	=> 'Washington Wizards'

		
);


$today = date("Ymd");
$yesterday  = date("Ymd", time() - 3600*24);
$codes = array($yesterday , $today);
	
 for ($row = 0; $row <= 1; $row++) {
 
	$day_code = $codes[$row];
	//echo " day code=" . $day_code;
	$url = "http://data.nba.com/5s/json/cms/noseason/scoreboard/" . $day_code . "/games.json";
	//echo $url;
	$json_str = file_get_contents($url);
	$json_array = json_decode($json_str, true);
	
	//print_r($json_array);
	if (isset($json_array['sports_content']['games']['game'])) {
		foreach ($json_array['sports_content']['games']['game'] as $key => $game){
		
			//print_r($game);
			$id = $game['id'];
			$game_url = $game['game_url'];
			$date = $game['date'];
			$time = $game['time'];
			$visitor = $game['visitor']['city'] . " " . $game['visitor']['nickname'];
			$v_score = $game['visitor']['score'];
			$home = $game['home']['city'] . " " . $game['home']['nickname'];
			$h_score = $game['home']['score'];
					
			//$p_period_value 	= $game['period_time']['period_value'];
			$p_period_status 	= $game['period_time']['period_status'];
			$p_clock 			= $game['period_time']['game_clock'];
			//$p_game_status 		= $game['period_time']['game_status'];
			
			$now = date("Y-m-d H:i:s");
			
			$home_id = array_search($home, $team_id);
			$away_id = array_search($visitor, $team_id);
			//echo "home: ", $home_id;
			//echo "away_id: ", $away_id;
			
			
			//check for existing record
			$game_data = $wpdb->get_row("select id, game_url, date, time, visitor, v_score, home, h_score, period, clock, dt_created, last_update, game_end, last_score, recap from p9_nba_scores where id = '$id'");						
			echo "0- ", $visitor, " at ", $home, "<br>"; 
						
			// insert into db
			if (empty($game_data->id)) { 
				echo "1-new ", $visitor, " at ", $home, "<br>";; 
				$wpdb->query( $wpdb->prepare( 
				"
				INSERT IGNORE INTO p9_nba_scores
				(id, game_url, date, time, visitor, visitor_id, v_score, home, home_id, h_score, period, clock, dt_created)
				VALUES ( %s, %s, %s, %s, %s, %d, %d, %s, %d, %d, %s, %s, %s, %s )
				", 
				$id,$game_url,$date,$time,$visitor,$away_id,$v_score,$home,$home_id,$h_score,$p_period_status,$p_clock,$now
				) );
			} 
			else { 
							
				echo "1-exists ", $visitor, " at ", $home, "<bR>";
				if ($game_data->game_end == "0000-00-00 00:00:00") {  
					echo "2-not over ", $visitor, " at ", $home, "<bR>";
					if (substr($p_period_status,0,5) == "Final") {
						// mark game over
						echo "2-over ", $visitor, " at ", $home, "<bR>";
						$sort = 3;
						$wpdb->query( $wpdb->prepare( 
						"
						UPDATE p9_nba_scores
						SET period=(%s), clock=(%s), v_score=(%d), h_score=(%d), last_update=(%s), game_end=(%s), home_id=%d, visitor_id=%d
						WHERE id=(%s)
						", 
						$p_period_status,$p_clock,$v_score,$h_score,$now,$now,$home_id,$away_id,
						$id
						) );
					}
					else if ($p_clock == "") {
						// check for change in quarter
						echo "3-live ", $visitor, " / ", $game_data->period, " / ", $p_period_status,  "<br>"; 
						if ($game_data->period == $p_period_status) {
							echo "3-nada ", $atn, " at ", $htn, "<br>"; 
						
							// no change, do nothing
						}
						else {
							echo "3-score ", $visitor, "-", $v_score, " at ", $home, "-", $h_score, "<BR>";
						
							$sort = 1;
							$wpdb->query( $wpdb->prepare( 
							"
							UPDATE p9_nba_scores
							SET period=(%s), clock=(%s), v_score=(%d), h_score=(%d), last_update=(%s)
							WHERE id=(%s)
							", 
							$p_period_status,$p_clock,$v_score,$h_score,$now,$home_id,$away_id,
							$id
							) );
							
						}
	
					}
					
				} // game end
				
				// get preview
				if (($game_data->preview == "") &&  ($game_data->v_score == "0") && ($game_data->h_score == "0"))  {
				
					echo "game preview <br>";
					$preview = substr(get_game_preview($game_data->game_url),0,200);
					echo $preview . "<BR>";
					if ($preview != "") {
						$wpdb->query( $wpdb->prepare( 
							"
							UPDATE p9_nba_scores
							SET preview=%s
							WHERE id=%s
							", 
							$preview,
							$id
						) );
						
						//echo $wpdb->last_query;		
					}
				
				}
								
										
				// post recap
				if (($game_data->game_end != "0000-00-00 00:00:00") && ($game_data->recap == "")) {  
				
					echo "game recap <br>";
					echo $game_data->game_url . "<br>";
					$recap = get_game_recap($game_data->game_url);
					echo $recap . "<BR>";
					if ($recap != "") {
						$wpdb->query( $wpdb->prepare( 
							"
							UPDATE p9_nba_scores
							SET recap=%s
							WHERE id=%s
							", 
							$recap,
							$id
						) );
						
						//echo $wpdb->last_query;		
					}
			
				
				}
				
				
			} // else		
	
		} // end for
	} // end if isset
}	// end for



// now load main reporting table for users
echo "<br><B>Loading data to main table</B><br>";
$now = date("Y-m-d H:i:s");
//$query 	= "SELECT deviceid, username FROM `p9_devices` where show_nba = 'Y'";
$query 	= "SELECT distinct deviceid, username  FROM `p9_devices_rss` where feed_id >= 1700 and feed_id < 1800 and active = 'Y'";
$rdevices = $wpdb->get_results($query) ;

if ($rdevices) {
	foreach( $rdevices as $rdevice ) {
	
		$user_array['u_deviceid']		= $rdevice->deviceid; 
		$user_array['u_userid']			= $rdevice->username;
		load_nba ($user_array, $wpdb, $debug);
	}
}
echo "<br>done<br>";




function get_game_recap($gameid) {


	$url = "http://www.nba.com/games/" . $gameid . "/gameinfo.html";
	
	
	$html = file_get_contents($url);
	//$html = json_decode($json_file);
	
	libxml_use_internal_errors(true);
	
	$dom = new DOMDocument;
	$dom->loadHTML($html);
	//$recap = $dom->getElementsByTagName('h3')->item(0);
	//ele = document.getElementById("theid");
	
	$recap = $dom->getElementById('nbaGIRecap2')->getElementsByTagName('h3')->item(0);
//	echo $recap->textContent;
	
	
	//die();
	
	//var_dump($recap);
	
	return $recap->textContent;

}



function get_game_preview($gameid) {


	$url = "http://www.nba.com/games/" . $gameid . "/gameinfo.html";
	
	
	$html = file_get_contents($url);
	//$html = json_decode($json_file);
	
	libxml_use_internal_errors(true);
	
	$dom = new DOMDocument;
	$dom->loadHTML($html);
	
	$preview = $dom->getElementById('nbaGIPreview')->getElementsByTagName('p')->item(0);
	//echo $preview->textContent;
	
	return $preview->textContent;




}

?>