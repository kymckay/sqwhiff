#pragma once
#include "src/sqf/sqf_type.h"
#include <unordered_map>
#include <string>

// Nullary keywords need to be differentiated to resolve grammar ambigutity that would otherwise arise (see issue #11)
const std::unordered_map<std::string, SQFType> SQF_Nullary_Keywords{
    {"acctime", SQFType::scalar},
    {"activatedaddons", SQFType::array},
    {"agents", SQFType::array},
    {"airdensitycurvertd", SQFType::array},
    {"all3denentities", SQFType::array},
    {"allactivetitleeffects", SQFType::array},
    {"alladdonsinfo", SQFType::array},
    {"allairports", SQFType::array},
    {"allcurators", SQFType::array},
    {"allcutlayers", SQFType::array},
    {"alldead", SQFType::array},
    {"alldeadmen", SQFType::array},
    {"alldisplays", SQFType::array},
    {"allgroups", SQFType::array},
    {"allmapmarkers", SQFType::array},
    {"allmines", SQFType::array},
    {"allplayers", SQFType::array},
    {"allsites", SQFType::array},
    {"allunits", SQFType::array},
    {"allunitsuav", SQFType::array},
    {"apertureparams", SQFType::boolean},
    {"armorypoints", SQFType::scalar},
    {"benchmark", SQFType::scalar},
    {"blufor", SQFType::side},
    {"break", SQFType::nothing},
    {"briefingname", SQFType::string},
    {"buldozer_isenabledroaddiag", SQFType::boolean},
    {"buldozer_reloadopermap", SQFType::nothing},
    {"cadetmode", SQFType::boolean},
    {"cameraon", SQFType::object},
    {"cameraview", SQFType::string},
    {"campaignconfigfile", SQFType::config},
    {"cansuspend", SQFType::boolean},
    {"cheatsenabled", SQFType::boolean},
    {"civilian", SQFType::side},
    {"clearforcesrtd", SQFType::nothing},
    {"clearitempool", SQFType::nothing},
    {"clearmagazinepool", SQFType::nothing},
    {"clearradio", SQFType::nothing},
    {"clearweaponpool", SQFType::nothing},
    {"clientowner", SQFType::scalar},
    {"commandingmenu", SQFType::string},
    {"configfile", SQFType::config},
    {"confignull", SQFType::config},
    {"continue", SQFType::nothing},
    {"controlnull", SQFType::control},
    {"copyfromclipboard", SQFType::string},
    {"createhashmap", SQFType::hashmap},
    {"curatorcamera", SQFType::object},
    {"curatormouseover", SQFType::object},
    {"curatorselected", SQFType::array},
    {"current3denoperation", SQFType::string},
    {"currentchannel", SQFType::scalar},
    {"currentnamespace", SQFType::name_space},
    {"cursorobject", SQFType::object},
    {"cursortarget", SQFType::object},
    {"customwaypointposition", SQFType::array},
    {"date", SQFType::array},
    {"daytime", SQFType::scalar},
    {"diag_activemissionfsms", SQFType::array},
    {"diag_activescripts", SQFType::array},
    {"diag_activesqfscripts", SQFType::array},
    {"diag_activesqsscripts", SQFType::array},
    {"diag_allmissioneventhandlers", SQFType::array},
    {"diag_deltatime", SQFType::scalar},
    {"diag_dumpcalltracetolog", SQFType::boolean},
    {"diag_dumpterrainsynth", SQFType::array},
    {"diag_fps", SQFType::scalar},
    {"diag_fpsmin", SQFType::scalar},
    {"diag_frameno", SQFType::scalar},
    {"diag_getterraingrid", SQFType::array},
    {"diag_resetfsm", SQFType::nothing},
    {"diag_resetshapes", SQFType::nothing},
    {"diag_scope", SQFType::scalar},
    {"diag_ticktime", SQFType::scalar},
    {"dialog", SQFType::boolean},
    {"diaryrecordnull", SQFType::task},
    {"didjip", SQFType::boolean},
    {"difficulty", SQFType::scalar},
    {"difficultyenabledrtd", SQFType::boolean},
    {"disabledebriefingstats", SQFType::nothing},
    {"disableserialization", SQFType::nothing},
    {"displaynull", SQFType::display},
    {"distributionregion", SQFType::scalar},
    {"dynamicsimulationsystemenabled", SQFType::boolean},
    {"east", SQFType::side},
    {"enableenddialog", SQFType::nothing},
    {"endl", SQFType::string},
    {"endloadingscreen", SQFType::nothing},
    {"environmentenabled", SQFType::array},
    {"environmentvolume", SQFType::scalar},
    {"estimatedendservertime", SQFType::scalar},
    {"exit", SQFType::nothing},
    {"false", SQFType::boolean},
    {"finishmissioninit", SQFType::nothing},
    {"fog", SQFType::scalar},
    {"fogforecast", SQFType::scalar},
    {"fogparams", SQFType::array},
    {"forcedmap", SQFType::array},
    {"forceend", SQFType::nothing},
    {"forceweatherchange", SQFType::nothing},
    {"freelook", SQFType::boolean},
    {"get3dencamera", SQFType::object},
    {"get3deniconsvisible", SQFType::array},
    {"get3denlinesvisible", SQFType::array},
    {"get3denmouseover", SQFType::array},
    {"getartillerycomputersettings", SQFType::array},
    {"getaudiooptionvolumes", SQFType::array},
    {"getcalculateplayervisibilitybyfriendly", SQFType::boolean},
    {"getclientstate", SQFType::string},
    {"getclientstatenumber", SQFType::string},
    {"getcursorobjectparams", SQFType::array},
    {"getdlcassetsusage", SQFType::array},
    {"getelevationoffset", SQFType::scalar},
    {"getlighting", SQFType::array},
    {"getloadedmodsinfo", SQFType::array},
    {"getmissiondlcs", SQFType::array},
    {"getmissionlayers", SQFType::array},
    {"getmouseposition", SQFType::array},
    {"getmusicplayedtime", SQFType::scalar},
    {"getobjectviewdistance", SQFType::array},
    {"getremotesensorsdisabled", SQFType::boolean},
    {"getresolution", SQFType::array},
    {"getshadowdistance", SQFType::scalar},
    {"getsteamfriendsservers", SQFType::array},
    {"getsubtitleoptions", SQFType::array},
    {"getterraingrid", SQFType::scalar},
    {"gettotaldlcusagetime", SQFType::scalar},
    {"groupiconselectable", SQFType::boolean},
    {"groupiconsvisible", SQFType::array},
    {"grpnull", SQFType::group},
    {"gusts", SQFType::scalar},
    {"halt", SQFType::nothing},
    {"hasinterface", SQFType::boolean},
    {"hcshownbar", SQFType::boolean},
    {"hudmovementlevels", SQFType::array},
    {"humidity", SQFType::scalar},
    {"independent", SQFType::side},
    {"initambientlife", SQFType::nothing},
    {"is3den", SQFType::boolean},
    {"is3denmultiplayer", SQFType::boolean},
    {"is3denpreview", SQFType::boolean},
    {"isactionmenuvisible", SQFType::boolean},
    {"isautotest", SQFType::boolean},
    {"isdedicated", SQFType::boolean},
    {"isfilepatchingenabled", SQFType::boolean},
    {"isgamefocused", SQFType::boolean},
    {"isgamepaused", SQFType::boolean},
    {"isinstructorfigureenabled", SQFType::boolean},
    {"ismultiplayer", SQFType::boolean},
    {"ismultiplayersolo", SQFType::boolean},
    {"ispipenabled", SQFType::boolean},
    {"isremoteexecuted", SQFType::boolean},
    {"isremoteexecutedjip", SQFType::boolean},
    {"isserver", SQFType::boolean},
    {"issteammission", SQFType::boolean},
    {"isstreamfriendlyuienabled", SQFType::boolean},
    {"isstressdamageenabled", SQFType::boolean},
    {"istuthintsenabled", SQFType::boolean},
    {"isuicontext", SQFType::boolean},
    {"language", SQFType::string},
    {"librarycredits", SQFType::array},
    {"librarydisclaimers", SQFType::array},
    {"lightnings", SQFType::scalar},
    {"linebreak", SQFType::text},
    {"loadgame", SQFType::nothing},
    {"localnamespace", SQFType::name_space},
    {"locationnull", SQFType::location},
    {"logentities", SQFType::nothing},
    {"mapanimclear", SQFType::nothing},
    {"mapanimcommit", SQFType::nothing},
    {"mapanimdone", SQFType::boolean},
    {"markasfinishedonsteam", SQFType::boolean},
    {"missionconfigfile", SQFType::config},
    {"missiondifficulty", SQFType::scalar},
    {"missionname", SQFType::string},
    {"missionnamesource", SQFType::string},
    {"missionnamespace", SQFType::name_space},
    {"missionstart", SQFType::array},
    {"missionversion", SQFType::scalar},
    {"moonintensity", SQFType::scalar},
    {"musicvolume", SQFType::scalar},
    {"netobjnull", SQFType::netobject},
    {"nextweatherchange", SQFType::scalar},
    {"nil", SQFType::any},
    {"objnull", SQFType::object},
    {"opencuratorinterface", SQFType::nothing},
    {"opfor", SQFType::side},
    {"overcast", SQFType::scalar},
    {"overcastforecast", SQFType::scalar},
    {"parsingnamespace", SQFType::name_space},
    {"particlesquality", SQFType::scalar},
    {"pi", SQFType::scalar},
    {"pixelgrid", SQFType::scalar},
    {"pixelgridbase", SQFType::scalar},
    {"pixelgridnouiscale", SQFType::scalar},
    {"pixelh", SQFType::scalar},
    {"pixelw", SQFType::scalar},
    {"playableunits", SQFType::array},
    {"player", SQFType::object},
    {"playerrespawntime", SQFType::scalar},
    {"playerside", SQFType::side},
    {"productversion", SQFType::array},
    {"profilename", SQFType::string},
    {"profilenamespace", SQFType::name_space},
    {"profilenamesteam", SQFType::string},
    {"radiovolume", SQFType::scalar},
    {"rain", SQFType::scalar},
    {"rainbow", SQFType::scalar},
    {"remoteexecutedowner", SQFType::scalar},
    {"resetcamshake", SQFType::nothing},
    {"resistance", SQFType::side},
    {"reversedmousey", SQFType::boolean},
    {"runinitscript", SQFType::nothing},
    {"safezoneh", SQFType::scalar},
    {"safezonew", SQFType::scalar},
    {"safezonewabs", SQFType::scalar},
    {"safezonex", SQFType::scalar},
    {"safezonexabs", SQFType::scalar},
    {"safezoney", SQFType::scalar},
    {"savegame", SQFType::nothing},
    {"savejoysticks", SQFType::nothing},
    {"saveprofilenamespace", SQFType::nothing},
    {"savingenabled", SQFType::boolean},
    {"scriptnull", SQFType::script},
    {"selectnoplayer", SQFType::nothing},
    {"servername", SQFType::string},
    {"servertime", SQFType::scalar},
    {"shownartillerycomputer", SQFType::boolean},
    {"shownchat", SQFType::boolean},
    {"showncompass", SQFType::boolean},
    {"showncuratorcompass", SQFType::boolean},
    {"showngps", SQFType::boolean},
    {"shownhud", SQFType::array},
    {"shownmap", SQFType::boolean},
    {"shownpad", SQFType::boolean},
    {"shownradio", SQFType::boolean},
    {"shownscoretable", SQFType::scalar},
    {"shownuavfeed", SQFType::boolean},
    {"shownwarrant", SQFType::boolean},
    {"shownwatch", SQFType::boolean},
    {"sideambientlife", SQFType::side},
    {"sideempty", SQFType::side},
    {"sideenemy", SQFType::side},
    {"sidefriendly", SQFType::side},
    {"sidelogic", SQFType::side},
    {"sideunknown", SQFType::side},
    {"simulweathersync", SQFType::nothing},
    {"slingloadassistantshown", SQFType::boolean},
    {"soundvolume", SQFType::scalar},
    {"speechvolume", SQFType::scalar},
    {"sunormoon", SQFType::scalar},
    {"switchableunits", SQFType::array},
    {"systemofunits", SQFType::scalar},
    {"systemtime", SQFType::array},
    {"systemtimeutc", SQFType::array},
    {"tasknull", SQFType::task},
    {"teammembernull", SQFType::team_member},
    {"teams", SQFType::array},
    {"teamswitch", SQFType::nothing},
    {"teamswitchenabled", SQFType::boolean},
    {"time", SQFType::scalar},
    {"timemultiplier", SQFType::scalar},
    {"true", SQFType::boolean},
    {"uinamespace", SQFType::name_space},
    {"userinputdisabled", SQFType::boolean},
    {"vehicles", SQFType::array},
    {"viewdistance", SQFType::scalar},
    {"visiblecompass", SQFType::boolean},
    {"visiblegps", SQFType::boolean},
    {"visiblemap", SQFType::boolean},
    {"visiblescoretable", SQFType::boolean},
    {"visiblewatch", SQFType::boolean},
    {"waves", SQFType::scalar},
    {"west", SQFType::side},
    {"wind", SQFType::array},
    {"winddir", SQFType::scalar},
    {"windrtd", SQFType::array},
    {"windstr", SQFType::scalar},
    {"worldname", SQFType::string},
    {"worldsize", SQFType::scalar},
};