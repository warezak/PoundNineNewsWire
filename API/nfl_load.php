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
		
	1500	=> 'Arizona Cardinals'
	,1501	=> 'Atlanta Falcons'
	,1502	=> 'Baltimore Ravens'
	,1503	=> 'Buffalo Bills'
	,1504	=> 'Carolina Panthers'
	,1505	=> 'Chicago Bears'
	,1506	=> 'Cincinnati Bengals'
	,1507	=> 'Cleveland Browns'
	,1508	=> 'Dallas Cowboys'
	,1509	=> 'Denver Broncos'
	,1510	=> 'Detroit Lions'
	,1511	=> 'Green Bay Packers'
	,1512	=> 'Houston Texans'
	,1513	=> 'Indianapolis Colts'
	,1514	=> 'Jacksonville Jaguars'
	,1515	=> 'Kansas City Chiefs'
	,1516	=> 'Miami Dolphins'
	,1517	=> 'Minnesota Vikings'
	,1518	=> 'New England Patriots'
	,1519	=> 'New Orleans Saints'
	,1520	=> 'New York Giants'
	,1521	=> 'New York Jets'
	,1522	=> 'Oakland Raiders'
	,1523	=> 'Philadelphia Eagles'
	,1524	=> 'Pittsburgh Steelers'
	,1525	=> 'San Diego Chargers'
	,1526	=> 'San Francisco 49ers'
	,1527	=> 'Seattle Seahawks'
	,1528	=> 'St. Louis Rams'
	,1529	=> 'Tampa Bay Buccaneers'
	,1530	=> 'Tennessee Titans'
	,1531	=> 'Washington Redskins'
					
		
);

// "get nfl scores";
//Make a request* to http://www.nfl.com/liveupdate/scorestrip/ss.xml during the regular season or http://www.nfl.com/liveupdate/scorestrip/postseason/ss.xml during the post season.
$feed_url = "http://www.nfl.com/liveupdate/scorestrip/ss.xml";
//$feed_url = "http://www.nfl.com/liveupdate/scorestrip/postseason/ss.xml";



libxml_use_internal_errors(true);
$RSS_DOC = simpleXML_load_file($feed_url);
if (!$RSS_DOC) {
	echo "Failed loading XML\n";
	foreach(libxml_get_errors() as $error) {
		echo "\t", $error->message;
	}
}
$gms_w = $RSS_DOC->gms['w'];
$gms_y = $RSS_DOC->gms['y'];
$gms_t = $RSS_DOC->gms['t'];
	
foreach($RSS_DOC->gms->g as $RSSitem)
{
	//<g eid="2014122801" gsis="56411" d="Sun" t="1:00" q="F" h="BAL" hnn="ravens" hs="20" v="CLE" vnn="browns" vs="10" rz="0" ga="" gt="REG"/>
	
	$eid 	= $RSSitem['eid'];
	$gsis 	= $RSSitem['gsis'];  // game id
	$d 		= $RSSitem['d'];	// day	of week
	$t 		= $RSSitem['t'];	// start time	
	$q 		= $RSSitem['q'];	// dame status
	$h 		= $RSSitem['h'];
	$hnn 	= $RSSitem['hnn']; 	//home nick name
	$hs 	= $RSSitem['hs'];	// home score
	$v 		= $RSSitem['v'];
	$vnn 	= $RSSitem['vnn'];	// visitor nick name
	$vs 	= $RSSitem['vs'];	// visitor score
	$rz 	= $RSSitem['rz']; //redzone
	$ga 	= $RSSitem['ga'];
	$gt 	= $RSSitem['gt']; //game type
	$k 		= $RSSitem['k']; //timeinquarter
	$p 		= $RSSitem['p']; //team wiht possession
	//$cnt++;
	
	$now = date("Y-m-d H:i:s");
	$vteam = get_team_name($v, $vnn);
	$hteam = get_team_name($h, $hnn);
	$home_id = array_search($hteam, $team_id);
	$away_id = array_search($vteam, $team_id);
	$gamestatus = getGameStatus($q);
	
	$year = substr($eid,0,4);	
	$month = substr($eid,4,2);
	$day = substr($eid,6,2);
	$game_day = $year . '-' . $month . '-' . $day;
	
	$link = $eid . "/" . $gms_y . "/" . $gms_t . $gms_w . "/" . $vnn . "@" . $hnn;
	//echo "link: " . $link . "\n";
	

	//check for existing record
	$game_data = $wpdb->get_row("select id, game_day, c0, c1, c2, c3, c4, visitor_id, c5, c6, home_id, c7, c8, c9, c10, c11, c12, c13, last_update, item_url, last_score, recap from p9_nfl_scores  where id = '$gsis'");						
	echo "<br>0- ", $vteam, " at ", $hteam, "<br>"; 
	//print_r($game_data);
	
	
	if (empty($game_data->id)) { 
		echo "  insert new ", $vteam, " at ", $hteam, "<br>";; 
		$wpdb->query( $wpdb->prepare( 
		"
		INSERT INTO p9_nfl_scores 
		(id, game_day, c0, c1, c2, c3, c4, visitor_id, c5, c6, home_id, c7, c8, c9, c10, c11, c12, c13, item_url)
		VALUES 
		(%s, %s, %s, %s, %s, %s, %s, %s,         %d, %s, %s,      %d, %s, %s, %s, %s, %s, %s, %s) 
		", 
		$gsis, $game_day, $d, $t, $gamestatus, $k, $vteam, $away_id, $vs, $hteam, $home_id, $hs, $p, $rz, $month, $day, $gt, $eid, $link
		) );
		echo $wpdb->last_query;
	}
	else {	
	
		//echo "recap" . $game_data->recap . "<BR>";
		//echo "gamestatus" . $gamestatus . "<BR>";
		// if game over, get final headline
		if (($game_data->recap == "") && (( $gamestatus == "Final") || ($gamestatus == "Final Overtime"))) {
		
			echo "game recap <br>";
			$recap = get_recap_headline($eid);
			echo $recap . "<BR>";
			if ($recap != "") {
				$wpdb->query( $wpdb->prepare( 
					"
					UPDATE p9_nfl_scores 
					SET  game_day=%s, c0=%s, c1=%s, c2=%s, c3=%s, c4=%s, visitor_id=%d, c5=%s, c6=%s, home_id=%d, c7=%s, c8=%s, c9=%s, c10=%s, c11=%s, c12=%s,
					c13=%s, item_url=%s, recap=%s, last_update=%s
					WHERE id=(%s)
					", 
					$game_day,$d,$t,$gamestatus,$k,$vteam,$away_id,$vs,$hteam,$home_id,$hs,$p, $rz, $month, $day, $gt, $eid, $link, $recap, $now,
					$gsis
				) );
				
				//echo $wpdb->last_query;		
			}
		
		
		}
	    //
		//
		//// check for change in score, could add change in game status / quarter / maybe possession
		else  if ((($game_data->c5 == $vs) && ($game_data->c7 == $hs)) || ($vs == "" || $hs == ""))  {
			echo "  game update, no score change ", $vteam, " / ", $game_data->c5, " / ", $vs, " at ", $home_team, " / ", $game_data->c7, " / ", $hs,  "<br>"; 
			//echo "  3-score no ", $vteam, " at ", $hteam, "<br>"; 
			
			// update, but not last_update field so that it does not go to top of stack
			$wpdb->query( $wpdb->prepare( 
					"
					UPDATE p9_nfl_scores 
					SET game_day=%s, c0=%s, c1=%s, c2=%s, c3=%s, c4=%s, visitor_id=%d, c5=%s, c6=%s, home_id=%d, c7=%s, c8=%s, c9=%s, c10=%s, c11=%s, c12=%s,
					c13=%s, item_url=%s					
					WHERE id=(%s)
					", 
					$game_day,$d,$t,$gamestatus,$k,$vteam,$away_id,$vs,$hteam,$home_id,$hs,$p, $rz, $month, $day, $gt, $eid, $link,
					$gsis
				) );
				
				//echo $wpdb->last_query;
		}
		else {
		
			echo "  score change ", $vteam, " at ", $hteam, "<br>"; 
			
			$last_score = get_score_details($eid);
			// update, include last update timestamp
			$wpdb->query( $wpdb->prepare( 
					"
					UPDATE p9_nfl_scores 
					SET game_day=%s, c0=%s, c1=%s, c2=%s, c3=%s, c4=%s, visitor_id=%d, c5=%s, c6=%s, home_id=%d, c7=%s, c8=%s, c9=%s, c10=%s, c11=%s, c12=%s,
					c13=%s, item_url=%s, last_update=%s, last_score=%s				
					WHERE id=(%s)
					", 
					$game_day,$d,$t,$gamestatus,$k,$vteam,$away_id,$vs,$hteam,$home_id,$hs,$p, $rz, $month, $day, $gt, $eid,  $link, $now, $last_score,
					$gsis
				) );
				
				//echo $wpdb->last_query;
		
		
			
		}

		
						
	}				
}




// now load main reporting table for users
echo "<br><B>Loading data to main table</B><br>";
$now = date("Y-m-d H:i:s");
//$query 	= "SELECT deviceid, username FROM `p9_devices` where show_nba = 'Y'";
$query 	= "SELECT distinct deviceid, username  FROM `p9_devices_rss` where feed_id >= 1500 and feed_id < 1600 and active = 'Y'";
$rdevices = $wpdb->get_results($query) ;

if ($rdevices) {
	foreach( $rdevices as $rdevice ) {
	
		$user_array['u_deviceid']		= $rdevice->deviceid; 
		$user_array['u_userid']			= $rdevice->username;
		load_nfl ($user_array, $wpdb, $debug);
	}
}
echo "<br>done<br>";


function get_score_details($eid) {

	
	//$eid = "2015092711";
	echo "-" . $eid . "-";
	settype($eid, "string");
	$url = "http://www.nfl.com/liveupdate/game-center/" . $eid . "/" . $eid . "_gtd.json";
	//echo $url;
	// copy file content into a string var
	$json_file = file_get_contents($url);
	// convert the string to a json object
	$jfo = json_decode($json_file,true);
	// read the title value
	//$jfo_temp = json_decode($json_file);
	
	
	//print_r($jfo_temp);
	//die();
	//$title = array();
	$scrsummary = $jfo[$eid]['scrsummary'];
	
	print_r($scrsummary);
	
	//echo "<br>";
	
	foreach ($scrsummary as $score) {
		
		print_r($score);
		
		//echo "sweet<BR>";
		$type 	= $score['type'];
		$team 	= $score['team'];
		$descr 	= $score['desc'];
		
		echo $team . ": " . $type . " " . $descr . "<br>";
		
		
		//$value = $value * 2;
	}
	
	$last_score =  $team . " "  .  $descr;
	return $last_score;
	

}


function get_recap_headline($eid) {



	$url = "http://www.nfl.com/widget/gc/2011/tabs/cat-post-recap-quick-take?gameId=" . $eid ;

	//ECHO $url;
	$html = file_get_contents($url);
	
	preg_match('/<h1>([^<]*)<\/h1>/iU', $html, $match);
	
	$headline =  strip_tags($match[0]);
	
	return $headline;



}



function get_team_name($teamshort, $default) {

	$teamname = "";
	
   switch ($teamshort) {
    case "NE":  $teamname = "New England Patriots"; break;	
	case "BUF": $teamname = "Buffalo Bills"; break;			
	case "MIA": $teamname = "Miami Dolphins"; break;			
	case "NYJ": $teamname = "New York Jets"; break;			
	case "PIT": $teamname = "Pittsburgh Steelers"; break;		
	case "CIN": $teamname = "Cincinnati Bengals"; break;		
	case "BAL": $teamname = "Baltimore Ravens"; break;		
	case "CLE": $teamname = "Cleveland Browns"; break;		
	case "IND": $teamname = "Indianapolis Colts"; break;		
	case "HOU": $teamname = "Houston Texans"; break;			
	case "JAC": $teamname = "Jacksonville Jaguars"; break;	
	case "TEN": $teamname = "Tennessee Titans"; break;		
	case "DEN": $teamname = "Denver Broncos"; break;			
	case "KC":  $teamname = "Kansas City Chiefs"; break;		
	case "SD":  $teamname = "San Diego Chargers"; break;		
	case "OAK": $teamname = "Oakland Raiders"; break;			
	case "DAL": $teamname = "Dallas Cowboys"; break;			
	case "PHI": $teamname = "Philadelphia Eagles"; break;		
	case "NYG": $teamname = "New York Giants"; break;			
	case "WAS": $teamname = "Washington Redskins"; break;		
	case "GB":  $teamname = "Green Bay Packers"; break;		
	case "DET": $teamname = "Detroit Lions"; break;			
	case "MIN": $teamname = "Minnesota Vikings"; break;		
	case "CHI": $teamname = "Chicago Bears"; break;			
	case "CAR": $teamname = "Carolina Panthers"; break;		
	case "NO":  $teamname = "New Orleans Saints"; break;		
	case "ATL": $teamname = "Atlanta Falcons"; break;			
	case "TB":  $teamname = "Tampa Bay Buccaneers"; break;	
	case "SEA": $teamname = "Seattle Seahawks"; break;		
	case "ARI": $teamname = "Arizona Cardinals"; break;		
	case "SF":  $teamname = "San Francisco 49ers"; break;		
	case "STL": $teamname = "St. Louis Rams"; break;	
	default: $teamname = $default;
   }
	
	return $teamname;

}

function getGameStatus($abbrGameStatus) {

  switch($abbrGameStatus) {
    case "P":
      $gameStatus = "Pregame";
      break;
    case "H":
      $gameStatus = "Halftime";
      break;
    case "5":
      $gameStatus = "Overtime";
      break;
    case "F":
      $gameStatus = "Final";
      break;
    case "FO":
      $gameStatus = "Final Overtime";
      break;
    default:
      $gameStatus = $abbrGameStatus;
  }
  return $gameStatus;
}









?>