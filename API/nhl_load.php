<?php


error_reporting(E_ALL);
//ini_set('display_startup_errors',1);
//ini_set('display_errors',1);
//error_reporting(-1);



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
		
		1800 =>	'Anaheim'
		,1801 =>	'Arizona'
		,1802 =>	'Boston'
		,1803 =>	'Buffalo'
		,1804 =>	'Calgary'
		,1805 =>	'Carolina'
		,1806 =>	'Chicago'
		,1807 =>	'Colorado'
		,1808 =>	'Columbus'
		,1809 =>	'Dallas'
		,1810 =>	'Detroit'
		,1811 =>	'Edmonton'
		,1812 =>	'Florida'
		,1813 =>	'Los Angeles'
		,1814 =>	'Minnesota'
		,1815 =>	'Montreal'
		,1825 =>	'Nashville'
		,1827 =>	'New Jersey'
		,1828 =>	'NY Islanders'
		,1829 =>	'NY Rangers'
		,1826 =>	'Ottawa'
		,1816 =>	'Philadelphia'
		,1817 =>	'Pittsburgh'
		,1818 =>	'San Jose'
		,1819 =>	'St Louis'
		,1820 =>	'Tampa Bay'
		,1821 =>	'Toronto'
		,1822 =>	'Vancouver'
		,1823 =>	'Washington'
		,1824 =>	'Winnipeg'
		
);

// "get nhl scores";
$json_str = file_get_contents('http://live.nhle.com/GameData/RegularSeasonScoreboardv3.jsonp');
$json_str = preg_replace( '/loadScoreboard\(/', '', $json_str );  
$json_str = preg_replace( '/\)/', '', $json_str );  
$json_array = json_decode($json_str, true);

$goodtags = array("id", "ts", "bs", "atn", "atv", "ats", "htn", "htv", "hts");

foreach ($json_array['games'] as $key=>$value){
			if (!is_array($value))
			{
				if ($debug == "Y") {
					echo $key ." => ". $value ."<BR>" ;
					}
			}
			else
			{
					foreach ($value as $key3 => $value3)
					{
						if (in_array($key3, $goodtags)) {
							if ($debug == 'Y') {echo $key3 ." => ". $value3. "\n" ; }
						}
						
							if ($key3 == "id") { $id = $value3; } 
							if ($key3 == "ts") { $ts = $value3; } 
							if ($key3 == "bs") { $bs = $value3; } 
							if ($key3 == "atn") { $atn = $value3; } 
							if ($key3 == "atv") { $atv = $value3; } 
							if ($key3 == "ats") { $ats = $value3; } 
							if ($key3 == "htn") { $htn = $value3; } 
							if ($key3 == "htv") { $htv = $value3; } 
							if ($key3 == "hts") { $hts = $value3; } 
										
					}
					
					//date_default_timezone_set('America/Los_Angeles');
					date_default_timezone_set('UTC');
					$now = date("Y-m-d H:i:s");
					$sort = 9;
					//
					//if ($ts != "TODAY") {
					//if (($bs != "LIVE" )&& ($bs != "FINAL")) {
				
					
					if ($bs != "LIVE" ) {
						
						if ($ts == "TODAY") {
							$sort = 2;
							$ts_date = date("Y-m-d");
							}
						else if (substr($bs,0,5) == "FINAL") {
							$sort = 3;
							$ts_split 	= explode(' ', $ts);	
							$ts_day  	= $ts_split[0];
							$ts_num  	= $ts_split[1];
							$ts_date	= $ts_num . "/" . date("Y");
							$time = strtotime($ts_date);
							$ts_date = date("Y-m-d",$time);
						}
						else
						{	
							$sort = 4;
							$ts_split 	= explode(' ', $ts);	
							$ts_day  	= $ts_split[0];
							$ts_num  	= $ts_split[1];
							$ts_date	= $ts_num . "/" . date("Y");
							$time = strtotime($ts_date);
							$ts_date = date("Y-m-d",$time);
						}
					}
					else {
						$sort = 1;
						$ts_date = date("Y-m-d");
					}
					
					// insert into db
					
					if ($htv == "mapleleafs") 	{$htv = "maple leafs";}
					if ($atv == "mapleleafs") 	{$atv = "maple leafs";}
					if ($htv == "bluejackets") 	{$htv = "blue jackets";}
					if ($atv == "bluejackets") 	{$atv = "blue jackets";}
					if ($htv == "redwings") 	{$htv = "red wings";}
					if ($atv == "redwings") 	{$atv = "red wings";}
					if ($htv == "blackhawks") 	{$htv = "black hawks";}
					if ($atv == "blackhawks") 	{$atv = "black hawks";}
					if ($htv == "rangers") 		{$htv = "";} // was showing NR Rangers rangers
					if ($atv == "rangers") 		{$atv = "";}
					if ($htv == "islanders") 	{$htv = "";} // was showing NY Islanders islanders
					if ($atv == "islanders") 	{$atv = "";}
					
					
					$home_id = array_search($htn, $team_id);
					$away_id = array_search($atn, $team_id);
					
					// if its final and not today, then do not update --- 
					//if ((strpos($bs,'FINAL') !== true) && (strpos($ts,'TODAY') !== false)) {
						
					//check for existing record
					$game_data = $wpdb->get_row("select id, game_day, ts, bs, atn, atv, ats, htn, htv, hts, last_update, sort, game_end, last_score, recap, preview from p9_nhl_scores where id = '$id'");						
					echo "0- ", $atn, " at ", $htn, "<br>"; 
					
						
					if (empty($game_data->id)) { 
							echo "1-new ", $atn, " at ", $htn, "<br>";; 
							$wpdb->query( $wpdb->prepare( 
							"
							INSERT INTO p9_nhl_scores
							(id, game_day, ts, bs, atn, visitor_id, atv, ats, htn, home_id, htv, hts, sort)
							VALUES ( %s, %s, %s, %s, %s, %d, %s, %d, %s, %d, %s, %d, %d )
							", 
							$id,$ts_date,$ts,$bs,$atn,$away_id,$atv,$ats,$htn,$home_id,$htv,$hts,$sort
							) );
						
						
					} 
					else { 
						
							
							echo "1-exists ", $atn, " at ", $htn, "<bR>";
							if ($game_data->game_end == "0000-00-00 00:00:00") {  
								//echo "2-not over ", $atn, " at ", $htn, "<bR>";
								
								if (substr($bs,0,5) == "FINAL") {
									// mark game over
									echo "game over ", $atn, " at ", $htn, "<bR>";
									$sort = 3;
									$wpdb->query( $wpdb->prepare( 
									"
									UPDATE p9_nhl_scores
									SET bs=(%s), ts=(%s), ats=(%d), hts=(%d), last_update=(%s), sort=(%d), game_end=(%s)
									WHERE id=(%s)
									", 
									$bs,$ts,$ats,$hts,$now,$sort,$now,
									$id
									) );
								} 
								else if ($bs == "LIVE") {
									// check for change in score
									echo "3-live ", $atn, " / ", $game_data->ats, " / ", $ats, " at ", $htn, " / ", $game_data->hts, " / ", $hts,  "<br>"; 
									if (((($game_data->hts == $hts) && ($game_data->ats == $ats)) || ($ats == "" || $hts == "")) && ($game_data->bs == $bs)) {
										echo "update clock ", $atn, " at ", $htn, "<br>"; 
									
										if ((empty($game_data->last_score)) && ($ats != "0" || $hts != "0")) { 
										
											echo "& last score <br>";
											$last_score = get_score_details($id);
											$wpdb->query( $wpdb->prepare( 
											"
											UPDATE p9_nhl_scores
											SET ts=(%s), last_score=%s
											WHERE id=(%s)
											", 
											$ts,$last_score,
											$id
											) );
											
										} else 
										// no change, update game clock
										{
											$sort = 1;
											$wpdb->query( $wpdb->prepare( 
											"
											UPDATE p9_nhl_scores
											SET ts=(%s)
											WHERE id=(%s)
											", 
											$ts,
											$id
											) );
										}
										
										
									}
									else {
										echo "score ", $atn, "-", $ats, " at ", $htn, "-", $hts, "<BR>";
										
										$last_score = get_score_details($id);
			
										$sort = 1;
										$wpdb->query( $wpdb->prepare( 
										"
										UPDATE p9_nhl_scores
										SET bs=(%s), ts=(%s), ats=(%d), hts=(%d), last_update=(%s), sort=(%d), last_score=%s
										WHERE id=(%s)
										", 
										$bs,$ts,$ats,$hts,$now,$sort,$last_score,
										$id
										) );
										
									}

								}	
								else {
									// game not started, preview
									if ($game_data->preview == "") {  
									
										echo "game preview <br>";
										$preview = get_game_preview($id);
										echo $preview . "<BR>";
										if ($preview != "") {
											$wpdb->query( $wpdb->prepare( 
												"
												UPDATE p9_nhl_scores
												SET preview=%s
												WHERE id=%s
												", 
												$preview,
												$id
											) );
											
											//echo $wpdb->last_query;		
										}
									
									}
								
								}
							} // game end
							
							// post recap
							if (($game_data->game_end != "0000-00-00 00:00:00") && ($game_data->recap == "")) {  
							
								echo "game recap <br>";
								$recap = get_game_recap($id);
								echo $recap . "<BR>";
								if ($recap != "") {
									$wpdb->query( $wpdb->prepare( 
										"
										UPDATE p9_nhl_scores
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
												
					//echo $wpdb->last_query;		
					
			} // if

}	// end json for loop		





// now load main reporting table for users
echo "<br><B>Loading data to main table</B><br>";
$now = date("Y-m-d H:i:s");
//$query 	= "SELECT deviceid, username FROM `p9_devices` where show_nba = 'Y'";
$query 	= "SELECT distinct deviceid, username  FROM `p9_devices_rss` where feed_id >= 1800 and feed_id < 1900 and active = 'Y'";
$rdevices = $wpdb->get_results($query) ;

if ($rdevices) {
	foreach( $rdevices as $rdevice ) {
	
		$user_array['u_deviceid']		= $rdevice->deviceid; 
		$user_array['u_userid']			= $rdevice->username;
		load_nhl ($user_array, $wpdb, $debug);
	}
}
echo "<br>done<br>";

function  get_score_details($id) { 


	$url = "http://live.nhle.com/GameData/20152016/" . $id . "/gc/gcbx.jsonp";
		
	// copy file content into a string var
	$json_str = file_get_contents($url);
	$json_str = preg_replace( '/GCBX.load\(/', '', $json_str );  
	$json_str = preg_replace( '/\}\)/', '}', $json_str );  
	//echo $json_str;
	
	$jfo = json_decode($json_str,true);
	//print_r($jfo);
	
	//$title = array();
	$scrsummary = $jfo['goalSummary'];
	
	foreach ($scrsummary as $period) {
		//echo "period";
		foreach ($period as $goals) {
			//echo "goals";
			if (is_array ($goals)) {
				foreach ($goals as $goal) {
					//echo "goal";
					//print_r($goal);
					$descr 	= $goal['desc'];
					$sip 	=  $goal['sip'];
					$p 		=  $goal['p'];
					echo $descr . "  P" . $p . " ";
					echo gmdate("i:s", $sip ) . "<br>";
				
				}
			}
		}
	}

	if (!empty($descr)) {
		//$last_score =  $descr . " P" . $p . " " . gmdate("i:s", $sip );
		$last_score =  $descr;
	}
	else 
	{	
		$last_score = "";
	}
	
	return $last_score;


}

function get_game_recap($gameid) {


	$url = "http://www.nhl.com/gamecenter/en/recap?id=" . $gameid;
	
	
	$html = file_get_contents($url);
	//$html = json_decode($json_file);
	
	libxml_use_internal_errors(true);
	
	$dom = new DOMDocument;
	$dom->loadHTML($html);
	$recap = $dom->getElementsByTagName('h1')->item(0);;
	
	// var_dump($books);
	
	return $recap->textContent;




}



function get_game_preview($gameid) {


	$url = "http://www.nhl.com/gamecenter/en/preview?id=" . $gameid;
	
	
	$html = file_get_contents($url);
	//$html = json_decode($json_file);
	
	libxml_use_internal_errors(true);
	
	$dom = new DOMDocument;
	$dom->loadHTML($html);
	$preview = $dom->getElementsByTagName('h1')->item(0);;
	
	// var_dump($books);
	
	return $preview->textContent;




}








?>