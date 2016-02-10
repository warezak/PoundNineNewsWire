/*
Author	: S.SENTHILRAJA
Company	: SEN Innovations
*/

#include <asf.h>
#include <util/delay.h>
#include <string.h>
#include "images/a49ers48x48.h"
#include "images/a76ers48x48.h"
#include "images/a9to5Mac48x48.h"
#include "images/ABC48x48.h"
#include "images/Adafruit48X48.h"
#include "images/AnandTech48x48.h"
#include "images/AndroidCentral48x48.h"
#include "images/Angels48x48.h"
#include "images/apnews48x48.h"
#include "images/Astros48x48.h"
#include "images/Athletics48x48.h"
#include "images/autoblog48x48.h"
#include "images/autoexpress48x48.h"
#include "images/Avalanche48x48.h"
#include "images/bbc48x48.h"
#include "images/bbcsport48x48.h"
#include "images/Bears48x48.h"
#include "images/Bengals48x48.h"
#include "images/bet48x48.h"
#include "images/BigThink48x48.h"
#include "images/Bills48x48.h"
#include "images/Blackhawks48x48.h"
#include "images/bleacherreports48x48.h"
#include "images/blooombergbusiness48x48.h"
#include "images/BlueJackets48x48.h"
#include "images/BlueJays48x48.h"
#include "images/Blues48x48.h"
#include "images/BoingBoing48x48.h"
#include "images/BrainyQuotes48x48.h"
#include "images/Braves48x48.h"
#include "images/Brewers48x48.h"
#include "images/Broncos48x48.h"
#include "images/Browns48x48.h"
#include "images/Bruins48x48.h"
#include "images/Buccaneers48x48.h"
#include "images/Bucks48x48.h"
#include "images/Bulls48x48.h"
#include "images/BusinessInsider48x48.h"
#include "images/BusinessWire48x48.h"
#include "images/buzzfeed48x48.h"
#include "images/Canadiens48x48.h"
#include "images/Canucks48x48.h"
#include "images/Capitals48x48.h"
#include "images/caranddriver48x48.h"
#include "images/Cardinals48x48.h"
#include "images/Cavaliers48x48.h"
#include "images/cbsnews48x48.h"
#include "images/Celtics48x48.h"
#include "images/Chargers48x48.h"
#include "images/Chiefs48x48.h"
#include "images/Clippers48x48.h"
#include "images/cnbc48x48.h"
#include "images/CNN48x48.h"
#include "images/CNNMoney48x48.h"
#include "images/Colts48x48.h"
#include "images/comicbook48x48.h"
#include "images/CoolHunting48x48.h"
#include "images/Cowboys48x48.h"
#include "images/Coyotes48x48.h"
#include "images/Cubs48x48.h"
#include "images/deadspin48x48.h"
#include "images/dealbreaker48x48.h"
#include "images/Devils48x48.h"
#include "images/Diamondbacks48x48.h"
#include "images/digitaltrends48x48.h"
#include "images/DiscoveryNews48x48.h"
#include "images/Dodgers48x48.h"
#include "images/Dolphins48x48.h"
#include "images/drudge48x48.h"
#include "images/Ducks48x48.h"
#include "images/Eagles48x48.h"
#include "images/engadget48x48.h"
#include "images/Entrepreneur48x48.h"
#include "images/EOnline48x48.h"
#include "images/espn48x48.h"
#include "images/extra48x48.h"
#include "images/Falcons48x48.h"
#include "images/Fashionista48x48.h"
#include "images/fastcompany48x48.h"
#include "images/Flames48x48.h"
#include "images/flavorwire48x48.h"
#include "images/food52-48x48.h"
#include "images/forbes48x48.h"
#include "images/foxnews48x48.h"
#include "images/foxsports48x48.h"
#include "images/Fubiz48x48.h"
#include "images/Gamasutra48x48.h"
#include "images/Giants48x48.h"
#include "images/Gizmodo48x48.h"
#include "images/googlenews48x48.h"
#include "images/GQ48x48.h"
#include "images/Grizzlies48x48.h"
#include "images/hackaday48x48.h"
#include "images/Hawks48x48.h"
#include "images/Heat48x48.h"
#include "images/Highsnobiety48x48.h"
#include "images/Hornets48x48.h"
#include "images/huffpost48x48.h"
#include "images/Hurricanes48x48.h"
#include "images/ign48x48.h"
#include "images/inccom48x48.h"
#include "images/Indians48x48.h"
#include "images/InsideHigherEd48x48.h"
#include "images/instructables48x48.h"
#include "images/investingcom48x48.h"
#include "images/io948x48.h"
#include "images/Islanders48x48.h"
#include "images/Jaguars48x48.h"
#include "images/Jazz48x48.h"
#include "images/Jets48x48.h"
#include "images/jezebel48x48.h"
#include "images/Kings48x48.h"
#include "images/Knicks48x48.h"
#include "images/Kotaku48x48.h"
#include "images/Lakers48x48.h"
#include "images/LaughingSquid48x48.h"
#include "images/lifhacker48x48.h"
#include "images/Lightning48x48.h"
#include "images/Lions48x48.h"
#include "images/livescience48x48.h"
#include "images/macrumors48x48.h"
#include "images/Magic48x48.h"
#include "images/MakeUseOf48x48.h"
#include "images/MapleLeafs48x48.h"
#include "images/Mariners48x48.h"
#include "images/MarketWatch48x48.h"
#include "images/Marketwired48x48.h"
#include "images/Marlins48x48.h"
#include "images/mashable48x48.h"
#include "images/Mavericks48x48.h"
#include "images/mentalfloss48x48.h"
#include "images/Mets48x48.h"
#include "images/mlb.h"
#include "images/motleyfool48x48.h"
#include "images/motortrend48x48.h"
#include "images/msnmoney48x48.h"
#include "images/nascar48x48.h"
#include "images/natgeo48x48.h"
#include "images/Nationals48x48.h"
#include "images/nba.h"
#include "images/nbc48x48.h"
#include "images/neatorama48x48.h"
#include "images/Nets48x48.h"
#include "images/newser48x48.h"
#include "images/nfl.h"
#include "images/NFLCardinals48x48.h"
#include "images/NFLGiants48x48.h"
#include "images/NFLJets48x48.h"
#include "images/nhl.h"
#include "images/NHLRangers48x48.h"
#include "images/NintendoLife48x48.h"
#include "images/NPR48x48.h"
#include "images/Nuggets48x48.h"
#include "images/nyt48x48.h"
#include "images/Oilers48x48.h"
#include "images/Orioles48x48.h"
#include "images/Pacers48x48.h"
#include "images/Packers48x48.h"
#include "images/Padres48x48.h"
#include "images/Panthers48x48.h"
#include "images/Patriots48x48.h"
#include "images/Pelicans48x48.h"
#include "images/Penguins48x48.h"
#include "images/people48x48.h"
#include "images/perezhilton48x48.h"
#include "images/PetaPixel48x48.h"
#include "images/Phillies48x48.h"
#include "images/physorg48x48.h"
#include "images/Pirates48x48.h"
#include "images/Pistons48x48.h"
#include "images/Politico48x48.h"
#include "images/popsugar48x48.h"
#include "images/Predators48x48.h"
#include "images/Quartz48x48.h"
#include "images/quotesnack48x48.h"
#include "images/quotespage48x48.h"
#include "images/Raiders48x48.h"
#include "images/Rams48x48.h"
#include "images/Rangers48x48.h"
#include "images/Raptors48x48.h"
#include "images/Ravens48x48.h"
#include "images/Rays48x48.h"
#include "images/readwrite48x48.h"
#include "images/reddit48x48.h"
#include "images/Reds48x48.h"
#include "images/Redskins48x48.h"
#include "images/RedSoxs48x48.h"
#include "images/RedWings48x48.h"
#include "images/reuters48x48.h"
#include "images/Rockets48x48.h"
#include "images/Rockies48x48.h"
#include "images/Royals48x48.h"
#include "images/Sabres48x48.h"
#include "images/Saints48x48.h"
#include "images/salon48x48.h"
#include "images/sciencedaily48x48.h"
#include "images/sciencemag48x48.h"
#include "images/Seahawks48x48.h"
#include "images/seekingalpha48x48.h"
#include "images/Sharks48x48.h"
#include "images/Siliconera48x48.h"
#include "images/skysports48x48.h"
#include "images/Slate48x48.h"
#include "images/spacecom48x48.h"
#include "images/Spurs48x48.h"
#include "images/Stars48x48.h"
#include "images/Steelers48x48.h"
#include "images/Suns48x48.h"
#include "images/TechCrunch48x48.h"
#include "images/Texans48x48.h"
#include "images/The Atlantic48x48.h"
#include "images/TheEconomist48x48.h"
#include "images/theregister48x48.h"
#include "images/thinkgeek48x48.h"
#include "images/ThoughtCatalog48x48.h"
#include "images/Thunder48x48.h"
#include "images/Tigers48x48.h"
#include "images/Timberwolves48x48.h"
#include "images/time48x48.h"
#include "images/Titans48x48.h"
#include "images/tmz48x48.h"
#include "images/TopSpeed48x48.h"
#include "images/TrailBlazers48x48.h"
#include "images/Twins48x48.h"
#include "images/Upworthy48s48.h"
#include "images/usatoday48x48.h"
#include "images/usweekly.h"
#include "images/usweekly48x48.h"
#include "images/variety48x48.h"
#include "images/VCDispatch48x48.h"
#include "images/VentureBeat48x48.h"
#include "images/vice48x48.h"
#include "images/Vikings48x48.h"
#include "images/Vulture48x48.h"
#include "images/Warriors48x48.h"
#include "images/washingtonpost48x48.h"
#include "images/webmd48x48.h"
#include "images/WhiteSox48x48.h"
#include "images/whowhatwear48x48.h"
#include "images/wikihow48x48.h"
#include "images/Wild48x48.h"
#include "images/wired48x48.h"
#include "images/Wizards48x48.h"
#include "images/WSJ.h"
#include "images/YahooEarnings48x48.h"
#include "images/YahooFinance48x48.h"
#include "images/YahooNews48x48.h"
#include "images/yahoosports48x48.h"
#include "images/Yankees48x48.h"
#include "images/twitterbird48x48.h"
#include "images/goldbox48x48.h"
#include "images/outlook48x48.h"
#include "images/gmail48x48.h"
#include "images/twitter.h"
#include "images/feedly.h"
#include "images/rotoworld48x48.h"
#include "images/scout48x48.h"

#include "flash_fs.h"
#include "window_manager.h"
#include "screens.h"

// Dataflash target sector used in this example
#define	TARGET_SECTOR 0x00002




/**
 * \brief Main application
 */

#define SLOTR

#ifdef SLOT27
#define NUM_ICONS_IN_FLASH 3
#elseif SLOTR
#define NUM_ICONS_IN_FLASH 1
#else 
#define NUM_ICONS_IN_FLASH 10
#endif

struct icon_file_t {
	gfx_color_t PROGMEM_PTR_T progmem_name;
	int length;
	int width;
};



int main(void)
{

	
	irq_initialize_vectors();

	board_init();
	sysclk_init();
	pmic_init();

	/* Make sure to initialize the display controller */
	gfx_init();

	/* Turn on the back light */
	ili9341_backlight_on();
	PORTC_INT0MASK=0x01; // 0000 0001
	PORTC_PIN0CTRL=0x1A;	 // X001 1010    //no invert, pull up , falling edge

	CLEAR_TRX_IRQ();
	ENABLE_TRX_IRQ();
	cpu_irq_enable();

	clr_scr();
	at45dbx_init();

int generated_file_name=0;

struct icon_file_t icon_file_list[NUM_ICONS_IN_FLASH]={
	
	/* only run 10 at a time */
		
#ifdef SLOT1
	
	{a49ers48x48             , 48, 48 }
	,{a76ers48x48             , 48, 48 }
	,{a9to5Mac48x48           , 48, 48 }
	,{ABC48x48                , 48, 48 }
	,{Adafruit48x48           , 48, 48 }
	,{AnandTech48x48          , 48, 48 }
	,{AndroidCentral48x48     , 48, 48 }
	,{Angels48x48             , 48, 48 }
	,{apnews48x48             , 48, 48 }
	,{Astros48x48             , 48, 48 }
#endif
#ifdef SLOT2

	{Athletics48x48          , 48, 48 }
	,{autoblog48x48           , 48, 48 }
	,{autoexpress48x48        , 48, 48 }
	,{Avalanche48x48          , 48, 48 }
	,{bbc48x48                , 48, 48 }
	,{bbcsport48x48           , 48, 48 }
	,{Bears48x48              , 48, 48 }
	,{Bengals48x48            , 48, 48 }
	,{bet48x48                , 48, 48 }
	,{BigThink48x48           , 48, 48 }
#endif
#ifdef SLOT3
	{Bills48x48              , 48, 48 }
	,{Blackhawks48x48         , 48, 48 }
	,{bleacherreports48x48    , 48, 48 }
	,{blooombergbusiness48x48 , 48, 48 }
	,{BlueJackets48x48        , 48, 48 }
	,{BlueJays48x48           , 48, 48 }
	,{Blues48x48              , 48, 48 }
	,{BoingBoing48x48         , 48, 48 }
	,{BrainyQuotes48x48       , 48, 48 }
	,{Braves48x48             , 48, 48 }
#endif
#ifdef SLOT4
	{Brewers48x48            , 48, 48 }
	,{Broncos48x48            , 48, 48 }
	,{Browns48x48             , 48, 48 }
	,{Bruins48x48             , 48, 48 }
	,{Buccaneers48x48         , 48, 48 }
	,{Bucks48x48              , 48, 48 }
	,{Bulls48x48              , 48, 48 }
	,{BusinessInsider48x48    , 48, 48 }
	,{BusinessWire48x48       , 48, 48 }
	,{buzzfeed48x48           , 48, 48 }
#endif
#ifdef SLOT5
	{Canadiens48x48          , 48, 48 }
	,{Canucks48x48            , 48, 48 }
	,{Capitals48x48           , 48, 48 }
	,{caranddriver48x48       , 48, 48 }
	,{Cardinals48x48          , 48, 48 }
	,{Cavaliers48x48          , 48, 48 }
	,{cbsnews48x48            , 48, 48 }
	,{Celtics48x48            , 48, 48 }
	,{Chargers48x48           , 48, 48 }
	,{Chiefs48x48             , 48, 48 }
#endif
#ifdef SLOT6
	{Clippers48x48           , 48, 48 }
	,{cnbc48x48               , 48, 48 }
	,{CNN48x48                , 48, 48 }
	,{CNNMoney48x48           , 48, 48 }
	,{Colts48x48              , 48, 48 }
	,{comicbook48x48          , 48, 48 }
	,{CoolHunting48x48        , 48, 48 }
	,{Cowboys48x48            , 48, 48 }
	,{Coyotes48x48            , 48, 48 }
	,{Cubs48x48               , 48, 48 }
#endif
#ifdef SLOT7

	{deadspin48x48           , 48, 48 }
	,{dealbreaker48x48        , 48, 48 }
	,{Devils48x48             , 48, 48 }
	,{Diamondbacks48x48       , 48, 48 }
	,{digitaltrends48x48      , 48, 48 }
	,{DiscoveryNews48x48      , 48, 48 }
	,{Dodgers48x48            , 48, 48 }
	,{Dolphins48x48           , 48, 48 }
	,{drudge48x48             , 48, 48 }
	,{Ducks48x48              , 48, 48 }
#endif
#ifdef SLOT8
	{Eagles48x48             , 48, 48 }
	,{engadget48x48           , 48, 48 }
	,{Entrepreneur48x48       , 48, 48 }
	,{EOnline48x48            , 48, 48 }
	,{espn48x48               , 48, 48 }
	,{extra48x48              , 48, 48 }
	,{Falcons48x48            , 48, 48 }
	,{Fashionista48x48        , 48, 48 }
	,{fastcompany48x48        , 48, 48 }
	,{Flames48x48             , 48, 48 }
#endif		
#ifdef SLOT9

	{food5248x48            , 48, 48 }
	,{forbes48x48             , 48, 48 }
	,{foxnews48x48            , 48, 48 }
	,{foxsports48x48          , 48, 48 }
	,{Fubiz48x48              , 48, 48 }
	,{Gamasutra48x48          , 48, 48 }
	,{Giants48x48             , 48, 48 }
	,{Gizmodo48x48            , 48, 48 }
	,{googlenews48x48         , 48, 48 }
	,{GQ48x48                 , 48, 48 }
#endif	
#ifdef SLOT10
	{hackaday48x48           , 48, 48 }
	,{Hawks48x48              , 48, 48 }
	,{Heat48x48               , 48, 48 }
	,{Highsnobiety48x48       , 48, 48 }
	,{Hornets48x48            , 48, 48 }
	,{huffpost48x48           , 48, 48 }
	,{Hurricanes48x48         , 48, 48 }
	,{ign48x48                , 48, 48 }
	,{inccom48x48             , 48, 48 }
	,{Indians48x48            , 48, 48 }
#endif		
#ifdef SLOT11
	{instructables48x48      , 48, 48 }
	,{investingcom48x48       , 48, 48 }
	,{io948x48                , 48, 48 }
	,{Islanders48x48          , 48, 48 }
	,{Jaguars48x48            , 48, 48 }
	,{Jazz48x48               , 48, 48 }
	,{Jets48x48               , 48, 48 }
	,{jezebel48x48            , 48, 48 }
	,{Kings48x48              , 48, 48 }
	,{Knicks48x48             , 48, 48 }

#endif
#ifdef SLOT12
	{Lakers48x48             , 48, 48 }
	,{LaughingSquid48x48      , 48, 48 }
	,{lifhacker48x48          , 48, 48 }
	,{Lightning48x48          , 48, 48 }
	,{Lions48x48              , 48, 48 }
	,{livescience48x48        , 48, 48 }
	,{macrumors48x48          , 48, 48 }
	,{Magic48x48              , 48, 48 }
	,{MakeUseOf48x48          , 48, 48 }
	,{flavorwire48x48         , 48, 48 }
#endif
	
#ifdef SLOT13
	{MapleLeafs48x48         , 48, 48 }
	,{Mariners48x48           , 48, 48 }
	,{MarketWatch48x48        , 48, 48 }
	,{Marketwired48x48        , 48, 48 }
	,{Marlins48x48            , 48, 48 }
	,{mashable48x48           , 48, 48 }
	,{Mavericks48x48          , 48, 48 }
	,{mentalfloss48x48        , 48, 48 }
	,{Mets48x48               , 48, 48 }
	,{mlb                     , 48, 48 }
#endif
#ifdef SLOT14
	{motleyfool48x48         , 48, 48 }
	,{motortrend48x48         , 48, 48 }
	,{msnmoney48x48           , 48, 48 }
	,{nascar48x48             , 48, 48 }
	,{natgeo48x48             , 48, 48 }
	,{Nationals48x48          , 48, 48 }
	,{nba                     , 48, 48 }
	,{nbc48x48                , 48, 48 }
	,{neatorama48x48          , 48, 48 }
	,{Nets48x48               , 48, 48 }
#endif
#ifdef SLOT15
	{newser48x48             , 48, 48 }
	,{nfl                     , 48, 48 }
	,{NFLCardinals48x48       , 48, 48 }
	,{NFLGiants48x48          , 48, 48 }
	,{NFLJets48x48            , 48, 48 }
	,{nhl                     , 48, 48 }
	,{NHLRangers48x48         , 48, 48 }
	,{NintendoLife48x48       , 48, 48 }
	,{NPR48x48                , 48, 48 }
	,{Nuggets48x48            , 48, 48 }
#endif
#ifdef SLOT16
	
	{nyt48x48                , 48, 48 }
	,{Oilers48x48             , 48, 48 }
	,{Orioles48x48            , 48, 48 }
	,{Pacers48x48             , 48, 48 }
	,{Packers48x48            , 48, 48 }
	,{Padres48x48             , 48, 48 }
	,{twitter	             , 48, 48 }
	,{feedly             , 48, 48 }
	,{Grizzlies48x48          , 48, 48 }
	,{Kotaku48x48             , 48, 48 }
#endif
#ifdef SLOT17
	{twitterbird48x48	           , 48, 48 }
	,{Patriots48x48           , 48, 48 }
	,{Pelicans48x48           , 48, 48 }
	,{Penguins48x48           , 48, 48 }
	,{people48x48             , 48, 48 }
	,{perezhilton48x48        , 48, 48 }
	,{PetaPixel48x48          , 48, 48 }
	,{Phillies48x48           , 48, 48 }
	,{physorg48x48            , 48, 48 }
	,{Pirates48x48            , 48, 48 }

#endif
#ifdef SLOT18	
	{Pistons48x48            , 48, 48 }
	,{Politico48x48           , 48, 48 }
	,{popsugar48x48           , 48, 48 }
	,{Predators48x48          , 48, 48 }
	,{Quartz48x48             , 48, 48 }
	,{quotesnack48x48         , 48, 48 }
	,{quotespage48x48         , 48, 48 }
	,{Raiders48x48            , 48, 48 }
	,{Rams48x48               , 48, 48 }
	,{Rangers48x48            , 48, 48 }
#endif

#ifdef SLOT19		 
	{Raptors48x48            , 48, 48 }
	,{Ravens48x48             , 48, 48 }
	,{Rays48x48               , 48, 48 }
	,{readwrite48x48          , 48, 48 }
	,{reddit48x48             , 48, 48 }
	,{Reds48x48               , 48, 48 }
	,{Redskins48x48           , 48, 48 }
	,{RedSoxs48x48            , 48, 48 }
	,{RedWings48x48           , 48, 48 }
	,{reuters48x48            , 48, 48 }
#endif		
#ifdef SLOT20
	{Rockets48x48            , 48, 48 }
	,{Rockies48x48            , 48, 48 }
	,{Royals48x48             , 48, 48 }
	,{Sabres48x48             , 48, 48 }
	,{Saints48x48             , 48, 48 }
	,{salon48x48              , 48, 48 }
	,{sciencedaily48x48       , 48, 48 }
	,{sciencemag48x48         , 48, 48 }
	,{Seahawks48x48           , 48, 48 }
	,{seekingalpha48x48       , 48, 48 }
#endif
#ifdef SLOT21

	{Sharks48x48             , 48, 48 }
	,{Siliconera48x48         , 48, 48 }
	,{skysports48x48          , 48, 48 }
	,{Slate48x48              , 48, 48 }
	,{spacecom48x48           , 48, 48 }
	,{Spurs48x48              , 48, 48 }
	,{Stars48x48              , 48, 48 }
	,{Steelers48x48           , 48, 48 }
	,{Suns48x48               , 48, 48 }
	,{TechCrunch48x48         , 48, 48 }
#endif
#ifdef SLOT22
	{Texans48x48             , 48, 48 }
	,{The_Atlantic48x48       , 48, 48 }
	,{TheEconomist48x48       , 48, 48 }
	,{theregister48x48        , 48, 48 }
	,{thinkgeek48x48          , 48, 48 }
	,{ThoughtCatalog48x48     , 48, 48 }
	,{Thunder48x48            , 48, 48 }
	,{Tigers48x48             , 48, 48 }
	,{Timberwolves48x48       , 48, 48 }
	,{time48x48               , 48, 48 }
#endif
#ifdef SLOT23
	{Titans48x48             , 48, 48 }
	,{tmz48x48                , 48, 48 }
	,{TopSpeed48x48           , 48, 48 }
	,{TrailBlazers48x48       , 48, 48 }
	,{Twins48x48              , 48, 48 }
	,{Upworthy48s48           , 48, 48 }
	,{usatoday48x48           , 48, 48 }
	,{usweekly                , 48, 48 }
	,{usweekly48x48           , 48, 48 }
	,{variety48x48            , 48, 48 }
#endif
#ifdef SLOT24
	{VCDispatch48x48         , 48, 48 }
	,{VentureBeat48x48        , 48, 48 }
	,{vice48x48               , 48, 48 }
	,{Vikings48x48            , 48, 48 }
	,{Vulture48x48            , 48, 48 }
	,{Warriors48x48           , 48, 48 }
	,{washingtonpost48x48     , 48, 48 }
	,{webmd48x48              , 48, 48 }
	,{WhiteSox48x48           , 48, 48 }
	,{whowhatwear48x48        , 48, 48 }
#endif
#ifdef SLOT25
	
	{wikihow48x48            , 48, 48 }
	,{Wild48x48               , 48, 48 }
	,{wired48x48              , 48, 48 }
	,{Wizards48x48            , 48, 48 }
	,{WSJ                     , 48, 48 }
	,{YahooEarnings48x48      , 48, 48 }
	,{YahooFinance48x48       , 48, 48 }
	,{YahooNews48x48          , 48, 48 }
	,{yahoosports48x48        , 48, 48 }
	,{Yankees48x48			, 48, 48 }
#endif
#ifdef SLOT26	
	{InsideHigherEd48x48     , 48, 48 }
	,{Panthers48x48           , 48, 48 }
	,{outlook48x48     , 48, 48 }
	,{gmail48x48           , 48, 48 }
	,{nbc48x48                , 48, 48 }
	,{popsugar48x48           , 48, 48 }
	,{mlb                , 48, 48 }
	,{nba                , 48, 48 }
	,{nfl                , 48, 48 }
	,{nhl                , 48, 48 }
#endif	
#ifdef SLOT27
	{goldbox48x48           , 48, 48 }
	,{scout48x48           , 48, 48 }
	,{rotoworld48x48           , 48, 48 }
#endif
#ifdef SLOTR
	{yahoosports48x48           , 48, 48 }
#endif	

	};
	

#ifdef SLOTR
 ffs_delete_file_from_inode(249); // deletes file 2
#endif	

// RUN clean only once
//ffs_clean_inode(); // cleans the filelist inside flash. (quick format)
//ffs_clean_sectors(10, 100); // used to clean sectors by passing the starting and ending sectors.
// there will be 4000 sectors in 2MB of flash memory
//ffs_delete_file_from_inode(2); // deletes file 2

	for (int i=0;i<NUM_ICONS_IN_FLASH;i++)
	{
		generated_file_name=ffs_write_icon_to_flash(icon_file_list[i].progmem_name, icon_file_list[i].length, icon_file_list[i].width);
	}
	screen_explorer();
	while(1);
}
