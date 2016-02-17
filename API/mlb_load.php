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
if ($debug == 'Y') {$wpdb->show_errors();}




// set team ids in array
$team_id = array(

		1619	=> 'Arizona D-backs'		
		,1615    => 'Atlanta Braves'		
		,1607    => 'Baltimore Orioles'	
		,1609    => 'Boston Red Sox'		
		,1618    => 'Chi Cubs'				
		,1613    => 'Chi White Sox'		
		,1628    => 'Cincinnati Reds'		
		,1605    => 'Cleveland Indians'	
		,1629    => 'Colorado Rockies'		
		,1611    => 'Detroit Tigers'		
		,1601    => 'Houston Astros'		
		,1610    => 'Kansas City Royals'	
		,1600    => 'LA Angels'			
		,1620    => 'LA Dodgers'			
		,1622    => 'Miami Marlins'		
		,1616    => 'Milwaukee Brewers'	
		,1612    => 'Minnesota Twins'		
		,1623    => 'NY Mets'				
		,1614    => 'NY Yankees'			
		,1602    => 'Oakland Athletics'	
		,1626    => 'Philadelphia Phillies'
		,1627    => 'Pittsburgh Pirates'	
		,1625    => 'San Diego Padres'		
		,1621    => 'San Francisco Giants'	
		,1606    => 'Seattle Mariners'		
		,1617    => 'St. Louis Cardinals'	
		,1604    => 'Tampa Bay Rays'		
		,1608    => 'Texas Rangers'		
		,1603    => 'Toronto Blue Jays'	
		,1624    => 'Washington Nationals'	
		
		
);



$year = isset($_GET['year']) ? $_GET['year'] : '';
$month = isset($_GET['month']) ? $_GET['month'] : '';
$day = isset($_GET['day']) ? $_GET['day'] : '';

// set to pst time to set date for getting scores
date_default_timezone_set('America/Los_Angeles');
$now = date("Y-m-d H:i:s");
if (empty($year)) { $year = date("Y"); }
if (empty($month)) { $month = date("m"); }
if (empty($day)) { $day = date("d"); }

$url = "http://gd2.mlb.com/components/game/mlb/year_" . $year . "/month_" . $month . "/day_" . $day . "/master_scoreboard.json";
//$recap_url = "http://mlb.mlb.com/gdcross/components/game/mlb/year_2015/month_09/day_28/gid_2015_09_28_oakmlb_anamlb_1/gamecenter.xml";
$recap_url = "http://mlb.mlb.com/gdcross/components/game/mlb/year_" . $year . "/month_" . $month . "/day_" . $day . "/gid_";

if ($debug == 'Y') {
	echo "year   ", $year, "\n";
	echo "month  ", $month, "\n";
	echo "day    ", $day, "\n";
	echo "url    ", $url, "\n";
	echo "\n";
}

// set back to utc for all insert timestamps
date_default_timezone_set('UTC');
$now = date("Y-m-d H:i:s");


// "get mlb scores";
//$json_str = file_get_contents('http://gd2.mlb.com/components/game/mlb/year_2015/month_03/day_10/master_scoreboard.json');
$json_str = file_get_contents($url);

//$json_str = preg_replace( '/loadScoreboard\(/', '', $json_str );  
//$json_str = preg_replace( '/\)/', '', $json_str );  
$json_array = json_decode($json_str, true);

//print_r($json_array);
$goodtags = array("id", "date", "period_status", "visitor", "score", "ats", "htn", "htv", "hts");

if (is_array($json_array)) {

  
    if (!isset($json_array['data']['games']['game']['0'])) {
	
	
		if (isset($json_array['data']['games']['game']) ){
			// only 1 game
			echo "1 game";
			
			$id 			= $json_array['data']['games']['game']['id'];
			$time 			= $json_array['data']['games']['game']['time'];
			$time_zone 		= $json_array['data']['games']['game']['time_zone'];
			$original_date 	= $json_array['data']['games']['game']['original_date'];
			$home_team 		= $json_array['data']['games']['game']['home_team_city'] . " " . $json_array['data']['games']['game']['home_team_name'];
			$away_team      = $json_array['data']['games']['game']['away_team_city'] . " " . $json_array['data']['games']['game']['away_team_name'];
			$venue 			= $json_array['data']['games']['game']['venue'];
			$hrhome  		= $json_array['data']['games']['game']['linescore']['hr']['home'];
			$hraway 		= $json_array['data']['games']['game']['linescore']['hr']['away'];
			$ehome  		= $json_array['data']['games']['game']['linescore']['e']['home'];
			$eaway 			= $json_array['data']['games']['game']['linescore']['e']['away'];
			$rhome  		= $json_array['data']['games']['game']['linescore']['r']['home'];
			$raway 			= $json_array['data']['games']['game']['linescore']['r']['away'];
			$sohome  		= $json_array['data']['games']['game']['linescore']['so']['home'];
			$soaway 		= $json_array['data']['games']['game']['linescore']['so']['away'];
			$sbhome  		= $json_array['data']['games']['game']['linescore']['sb']['home'];
			$sbaway 		= $json_array['data']['games']['game']['linescore']['sb']['away'];
			$hhome  		= $json_array['data']['games']['game']['linescore']['h']['home'];
			$haway 			= $json_array['data']['games']['game']['linescore']['h']['away'];
			$inning  		= $json_array['data']['games']['game']['status']['inning'];		
			$inning_state	= $json_array['data']['games']['game']['status']['inning_state'];		
			$status  		= $json_array['data']['games']['game']['status']['status'];		
			
			$home_probable_pitcher  		= $json_array['data']['games']['game']['home_probable_pitcher']['name_display_roster'];
			$away_probable_pitcher  		= $json_array['data']['games']['game']['opposing_pitcher']['name_display_roster'];
			
			process_game($wpdb,$team_id,$recap_url,$id,$time,$time_zone,$original_date,$home_team,$away_team,$venue,$hrhome,$hraway,$ehome,$eaway,$rhome,$raway,$sohome ,$soaway,$sbhome,$sbaway,$hhome ,$haway,$inning,$inning_state,$status ,$home_probable_pitcher, $away_probable_pitcher);		
			
			
			
  
		}
  
	}	else	{
		// multiple games

		//foreach ($json_array['sports_content']['games']['game'] as $key => $value){
		foreach ($json_array['data']['games']['game'] as $key => $game){
		
			//echo $key ." => ". $value ."<BR>" ;
			//if (in_array($key3, $goodtags)) {
			//echo "ok";
			//print_r($game);
			//die();
			$id 			= $game['id'];
			$time 			= $game['time'];
			$time_zone 		= $game['time_zone'];
			$original_date 	= $game['original_date'];
			$home_team 		= $game['home_team_city'] . " " . $game['home_team_name'];
			$away_team      = $game['away_team_city'] . " " . $game['away_team_name'];
			$venue 			= $game['venue'];
			$hrhome  		= $game['linescore']['hr']['home'];
			$hraway 		= $game['linescore']['hr']['away'];
			$ehome  		= $game['linescore']['e']['home'];
			$eaway 			= $game['linescore']['e']['away'];
			$rhome  		= $game['linescore']['r']['home'];
			$raway 			= $game['linescore']['r']['away'];
			$sohome  		= $game['linescore']['so']['home'];
			$soaway 		= $game['linescore']['so']['away'];
			$sbhome  		= $game['linescore']['sb']['home'];
			$sbaway 		= $game['linescore']['sb']['away'];
			$hhome  		= $game['linescore']['h']['home'];
			$haway 			= $game['linescore']['h']['away'];
			
			$inning  		= $game['status']['inning'];		
			$inning_state	= $game['status']['inning_state'];		
			$status  		= $game['status']['status'];		
			
			$home_probable_pitcher  		= $game['home_probable_pitcher']['name_display_roster'];
			$away_probable_pitcher  		= $game['away_probable_pitcher']['name_display_roster'];
			
			process_game($wpdb,$team_id,$recap_url,$id,$time,$time_zone,$original_date,$home_team,$away_team,$venue,$hrhome,$hraway,$ehome,$eaway,$rhome,$raway,$sohome ,$soaway,$sbhome,$sbaway,$hhome ,$haway,$inning,$inning_state,$status ,$home_probable_pitcher, $away_probable_pitcher);		
			
			
			
			
			
			
			
			
			
			
		} // end for each
	}
}

// now load main reporting table for users
echo "<br><B>Loading data to main table</B><br>";
$now = date("Y-m-d H:i:s");
//$query 	= "SELECT deviceid, username FROM `p9_devices` where show_mlb = 'Y'";
$query 	= "SELECT distinct deviceid, username  FROM `p9_devices_rss` where feed_id >= 1600 and feed_id < 1700 and active = 'Y'";
$rdevices = $wpdb->get_results($query) ;

if ($rdevices) {
	foreach( $rdevices as $rdevice ) {
	
		$user_array['u_deviceid']		= $rdevice->deviceid; 
		$user_array['u_userid']			= $rdevice->username;
		load_mlb ($user_array, $wpdb, $debug);
	}
}
echo "<br>done<br>";

function process_game($wpdb,$team_id,$recap_url,$id,$time,$time_zone,$original_date,$home_team,$away_team,$venue,$hrhome,$hraway,$ehome,$eaway,$rhome,$raway,$sohome ,$soaway,$sbhome,$sbaway,$hhome ,$haway,$inning,$inning_state,$status ,$home_probable_pitcher, $away_probable_pitcher) {

			
			$now = date("Y-m-d H:i:s");


			if (($status == "Final") || ($status == "Game Over") ){
						$sort 	= 3;
				} else if (( $status == "Preview" ) || ( $status == "Warmup" ) || ( $status == "Pre-Game" )) {
						$sort = 2;
				}
				else {
						$sort = 1;
			}
			
			$inning = cleaninning($inning);
			
					
			if ($debug == 'Y') {
				echo "id                 ", $id, "\n"; 
				echo "date               ", $original_date, "\n"; 
				echo "time               ", $time, "\n"; 			
				echo "time_zone          ", $time_zone, "\n";	
				echo "home_team          ", $home_team, "\n";		
				echo "away_team          ", $away_team, "\n"; 
				echo "venue              ", $venue, "\n";		
				
				echo "inning             ", $inning, "\n";	
				echo "inning state       ", $inning_state, "\n";
				echo "status             ", $status, "\n";		
				
				echo "away runs          ", $raway, "\n";	
				echo "home runs          ", $rhome, "\n";	
				echo "away hits          ", $haway, "\n";	
				echo "home hits          ", $hhome, "\n";	
				echo "away errors        ", $eaway, "\n";	
				echo "home errors        ", $ehome, "\n";	
				
				echo "hrhome             ", $hrhome, "\n";	
				echo "hraway             ", $hraway, "\n";	
				echo "sohome             ", $sohome, "\n";	
				echo "soaway             ", $soaway, "\n";	
				echo "sbhome             ", $sbhome, "\n";	
				echo "sbaway             ", $sbaway, "\n";	
				
				echo "home_pitcher       ", $home_probable_pitcher, "\n";	
				echo "away_pitcher       ", $away_probable_pitcher, "\n";	
				
				
				echo "\n";
			}
			
			// some clean up
			if ($home_team == "NY Yankees Yankees") 		{ $home_team = "NY Yankees";}
			if ($away_team == "NY Yankees Yankees") 		{ $away_team = "NY Yankees";}
			if ($home_team == "NY Mets Mets") 				{ $home_team = "NY Mets";}
			if ($away_team == "NY Mets Mets") 				{ $away_team = "NY Mets";}
			if ($home_team == "Chi Cubs Cubs") 				{ $home_team = "Chi Cubs";}
			if ($away_team == "Chi Cubs Cubs") 				{ $away_team = "Chi Cubs";}
			if ($home_team == "Chi White Sox White Sox") 	{ $home_team = "Chi White Sox";}
			if ($away_team == "Chi White Sox White Sox") 	{ $away_team = "Chi White Sox";}
			if ($home_team == "LA Dodgers Dodgers") 		{ $home_team = "LA Dodgers";}
			if ($away_team == "LA Dodgers Dodgers") 		{ $away_team = "LA Dodgers";}
			if ($home_team == "LA Angels Angels") 			{ $home_team = "LA Angels";}
			if ($away_team == "LA Angels Angels") 			{ $away_team = "LA Angels";}
			
			$home_id = array_search($home_team, $team_id);
			$away_id = array_search($away_team, $team_id);
			
			
			// insert into db		
			
			//check for existing record
			$game_data = $wpdb->get_row("select id, date, time, timezone, visitor, home, v_score, h_score, v_hits, h_hits, v_errors, h_errors, v_pitcher_prob, h_pitcher_prob, inning, inning_state, status, dt_created, last_update, sort, game_end, last_score, recap from p9_mlb_scores  where id = '$id'");						
			echo "0- ", $away_team, " at ", $home_team, "<br>"; 
			
			if (empty($game_data->id)) { 
					echo "1-new ", $away_team, " at ", $home_team, "<br>";; 
					$wpdb->query( $wpdb->prepare( 
					"
					INSERT INTO p9_mlb_scores
					(id, date, time, timezone, visitor, visitor_id, home, home_id, v_score, h_score, v_hits, h_hits, v_errors, h_errors, v_pitcher_prob, h_pitcher_prob, inning, inning_state, status, dt_created, sort)
					VALUES ( %s, %s, %s, %s, %s, %s,  %s, %s, %d, %d, %d, %d, %d, %d, %s, %s, %d, %s, %s, %s, %s, %d )
					", 
					$id,$original_date,$time,$time_zone,$away_team,$away_id,$home_team,$home_id,$raway,$rhome,$haway,$hhome,$eaway,$ehome,$away_probable_pitcher,$home_probable_pitcher,$inning,$inning_state,$status,$now,$sort
					) );
			} 
			else { 
							
					echo "1-exists ", $atn, " at ", $htn, "<bR>";
					if ($game_data->game_end == "0000-00-00 00:00:00") {  
						echo "2-not over ", $away_team, " at ", $home_team, "<bR>";
						
						if (($status == "Final") || ($status == "Game Over") || ($status == "Postponed")){
							// mark game over
							echo "2-over ", $away_team, " at ", $home_team, "<bR>";
							$sort = 3;
							$wpdb->query( $wpdb->prepare( 
							"
							UPDATE p9_mlb_scores
							SET v_score=%d,  h_score=%d, v_hits=%d, h_hits=%d, v_errors=%d, h_errors=%d,
							inning=%s, inning_state=%s, status=%s, last_update=%s, sort=%d, game_end=%s
							WHERE id=(%s)
							", 
							$raway,$rhome,$haway,$hhome,$eaway,$ehome,$inning,$inning_state,$status,$now,$sort,$now,
							$id
							) );
						} 	
	
						else {
							// check for change in score
							echo "3-score? ", $away_team, " / ", $game_data->v_score, " / ", $raway, " at ", $home_team, " / ", $game_data->h_score, " / ", $rhome,  "<br>"; 
							if ((($game_data->h_score == $rhome) && ($game_data->v_score == $raway)) || ($raway == "" || $rhome == ""))  {
								echo "3-score no change ", $away_team, " at ", $home_team, "<br>"; 
							
								// check for pitcher change 
								echo "3-pitcher? ", $away_team, " / ", $game_data->v_pitcher_prob, " / ", $away_probable_pitcher, " at ", $home_team, " / ", $game_data->h_pitcher_prob, " / ", $home_probable_pitcher,  "<br>"; 
								if (($game_data->v_pitcher_prob != $away_probable_pitcher) || ($game_data->h_pitcher_prob != $home_probable_pitcher))  {
									echo "3-yes pitcher ", $away_team, "-", $raway, " at ", $home_team, "-", $rhome, "<BR>";
									$wpdb->query( $wpdb->prepare( 
									"
									UPDATE p9_mlb_scores
									SET v_pitcher_prob=%s,  h_pitcher_prob=%s
									WHERE id=(%s)
									", 
									$away_probable_pitcher,$home_probable_pitcher,
									$id
									) );
									
									echo $wpdb->last_query;	
									
								}
								// if last score not posted, get it
								if ((empty($game_data->last_score)) && (($raway != "0" || $rhome != "0") && ($raway != "" || $rhome != ""))) { 
											
									echo "& last score <br>";
									$last_score = get_score_details($id);
									$wpdb->query( $wpdb->prepare( 
									"
									UPDATE p9_mlb_scores
									SET v_score=%d,  h_score=%d, v_hits=%d, h_hits=%d, v_errors=%d, h_errors=%d,
									inning=%s, inning_state=%s, status=%s, last_score=%s
									WHERE id=(%s)
									", 
									$raway,$rhome,$haway,$hhome,$eaway,$ehome,$inning,$inning_state,$status,$last_score,
									$id
									) );
								}
								else 
								{
									// no change, update all but pubdate, which will prevent it from getting pushed to top of stack.
								
									$wpdb->query( $wpdb->prepare( 
									"
									UPDATE p9_mlb_scores
									SET v_score=%d,  h_score=%d, v_hits=%d, h_hits=%d, v_errors=%d, h_errors=%d,
									inning=%s, inning_state=%s, status=%s
									WHERE id=(%s)
									", 
									$raway,$rhome,$haway,$hhome,$eaway,$ehome,$inning,$inning_state,$status,
									$id
									) );
								}
								
								
													
							}
							else {
								echo "3-yes score ", $away_team, "-", $raway, " at ", $home_team, "-", $rhome, "<BR>";
							
								$last_score = get_score_details($id);
								$sort = 1;
								$wpdb->query( $wpdb->prepare( 
								"
								UPDATE p9_mlb_scores
								SET v_score=%d,  h_score=%d, v_hits=%d, h_hits=%d, v_errors=%d, h_errors=%d,
								inning=%s, inning_state=%s, status=%s, last_update=%s, sort=%d, last_score=%s
								WHERE id=(%s)
								", 
								$raway,$rhome,$haway,$hhome,$eaway,$ehome,$inning,$inning_state,$status,$now,$sort,$last_score,
								$id
							) );
								
							}
	
						}								
					} // game end
					
					// post recap
					if (($game_data->game_end != "0000-00-00 00:00:00") && ($game_data->recap == "")) {  
					
						echo "game recap <br>";
						$recap_id		= "";
						$game_recap_url	= "";
						$recap_id 		= str_replace('/', '_', $id);
						$recap_id 		= str_replace('-', '_', $recap_id);
						///gid_2015_09_28_oakmlb_anamlb_1/gamecenter.xml";
						$game_recap_url		= $recap_url . $recap_id . "/gamecenter.xml";
						$recap 			= get_game_recap($game_recap_url);
						
						echo $recap . "<BR>";
						if ($recap != "") {
							$wpdb->query( $wpdb->prepare( 
								"
								UPDATE p9_mlb_scores
								SET recap=%s
								WHERE id=%s
								", 
								$recap,
								$id
							) );
							
							//echo $wpdb->last_query;		
						}
			
					
					}
								
			}			




}

function  get_score_details($id) { 
	
	date_default_timezone_set('America/Los_Angeles');
	$year = date("Y"); 
	$month = date("m"); 
	$day = date("d");
	$score_url 		= "http://mlb.mlb.com/gdcross/components/game/mlb/year_" . $year . "/month_" . $month . "/day_" . $day . "/gid_";
	$game_id 		= str_replace('/', '_', $id);
	$game_id 		= str_replace('-', '_', $game_id);
	///gid_2015_09_28_oakmlb_anamlb_1/gamecenter.xml";
	$score_url		= $score_url . $game_id . "/inning/inning_Scores.xml?live";
	
	//echo $score_url;
	
	$html = file_get_contents($score_url);
	libxml_use_internal_errors(true);
	$MLB_DOC = simplexml_load_string($html);
	    
	if (!$MLB_DOC) {
		echo "Failed loading XML\n";
		foreach(libxml_get_errors() as $error) {
			echo "\t", $error->message;
		}
	}
	else 
	{
		//$scrsummary = $MLB_DOC->scores;
		//print_r($MLB_DOC);
		//echo "<BR>";
		foreach($MLB_DOC->score as $score)
		{
				$desc = $score['pbp'];
				//echo "score: " . $desc . "<BR>";
		}
		
	}

	return $desc;
}


function get_game_recap($url) {

		echo $url;
		$html = file_get_contents($url);
		libxml_use_internal_errors(true);
		$MLB_DOC = simplexml_load_string($html);
	    
		if (!$MLB_DOC) {
			echo "Failed loading XML\n";
			foreach(libxml_get_errors() as $error) {
				echo "\t", $error->message;
			}
			
		}
		
		else 

		{
				
			$headline = $MLB_DOC->wrap->mlb->headline;
			
		}
			
		return $headline;

}



function cleaninning($inning) {

	switch ($inning) {
			case 1:
					$inning = "1st";
					break;
			case 2:
					$inning = "2nd";
					break;
			case 3:
					$inning = "3rd";
					break;
			case 4:
					$inning = "4th";
					break;
			case 5:
					$inning = "5th";
					break;
			case 6:
					$inning = "6th";
					break;
			case 7:
					$inning = "7th";
					break;
			case 8:
					$inning = "8th";
					break;
			case 9:
					$inning = "9th";
					break;
			case 10:
					$inning = "10th";
					break;
			case 11:
					$inning = "11th";
					break;
			case 12:
					$inning = "12th";
					break;
			case 13:
					$inning = "13th";
					break;
			case 14:
					$inning = "14th";
					break;
			case 15:
					$inning = "15th";
					break;
					
		}
		
		return $inning;
}

?>

