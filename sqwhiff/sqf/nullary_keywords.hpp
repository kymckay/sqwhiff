#pragma once
#include <string>
#include <unordered_map>

#include "sqwhiff/sqf/type.hpp"

namespace SQF {

const std::unordered_map<std::string, Type> Nullary_Keywords{
    {"acctime", Type::scalar},
    {"activatedaddons", Type::array},
    {"agents", Type::array},
    {"airdensitycurvertd", Type::array},
    {"all3denentities", Type::array},
    {"allactivetitleeffects", Type::array},
    {"alladdonsinfo", Type::array},
    {"allairports", Type::array},
    {"allcurators", Type::array},
    {"allcutlayers", Type::array},
    {"alldead", Type::array},
    {"alldeadmen", Type::array},
    {"alldisplays", Type::array},
    {"allenv3dsoundsources", Type::array},
    {"allgroups", Type::array},
    {"allmapmarkers", Type::array},
    {"allmines", Type::array},
    {"allplayers", Type::array},
    {"allsites", Type::array},
    {"allunits", Type::array},
    {"allunitsuav", Type::array},
    {"allusers", Type::array},
    {"ambienttemperature", Type::array},
    {"apertureparams", Type::array},
    {"armorypoints", Type::scalar},
    {"benchmark", Type::scalar},
    {"blufor", Type::side},
    {"break", Type::nothing},
    {"briefingname", Type::string},
    {"buldozer_isenabledroaddiag", Type::boolean},
    {"buldozer_reloadopermap", Type::nothing},
    {"cadetmode", Type::boolean},
    {"cameraon", Type::object},
    {"cameraview", Type::string},
    {"campaignconfigfile", Type::config},
    {"cansuspend", Type::boolean},
    {"cheatsenabled", Type::boolean},
    {"civilian", Type::side},
    {"clearforcesrtd", Type::nothing},
    {"clearitempool", Type::nothing},
    {"clearmagazinepool", Type::nothing},
    {"clearradio", Type::nothing},
    {"clearweaponpool", Type::nothing},
    {"clientowner", Type::scalar},
    {"commandingmenu", Type::string},
    {"configfile", Type::config},
    {"confignull", Type::config},
    {"continue", Type::nothing},
    {"controlnull", Type::control},
    {"copyfromclipboard", Type::string},
    {"createhashmap", Type::hashmap},
    {"curatorcamera", Type::object},
    {"curatormouseover", Type::object},
    {"curatorselected", Type::array},
    {"current3denoperation", Type::string},
    {"currentchannel", Type::scalar},
    {"currentnamespace", Type::name_space},
    {"cursorobject", Type::object},
    {"cursortarget", Type::object},
    {"customwaypointposition", Type::array},
    {"date", Type::array},
    {"daytime", Type::scalar},
    {"diag_activemissionfsms", Type::array},
    {"diag_activescripts", Type::array},
    {"diag_activesqfscripts", Type::array},
    {"diag_activesqsscripts", Type::array},
    {"diag_allmissioneventhandlers", Type::array},
    {"diag_deltatime", Type::scalar},
    {"diag_fps", Type::scalar},
    {"diag_fpsmin", Type::scalar},
    {"diag_frameno", Type::scalar},
    {"diag_scope", Type::scalar},
    {"diag_stacktrace", Type::array},
    {"diag_ticktime", Type::scalar},
    {"dialog", Type::boolean},
    {"diaryrecordnull", Type::task},
    {"didjip", Type::boolean},
    {"difficulty", Type::scalar},
    {"difficultyenabledrtd", Type::boolean},
    {"disabledebriefingstats", Type::nothing},
    {"disableserialization", Type::nothing},
    {"displaynull", Type::display},
    {"distributionregion", Type::scalar},
    {"dynamicsimulationsystemenabled", Type::boolean},
    {"east", Type::side},
    {"enableenddialog", Type::nothing},
    {"endl", Type::string},
    {"endloadingscreen", Type::nothing},
    {"environmentenabled", Type::array},
    {"environmentvolume", Type::scalar},
    {"estimatedendservertime", Type::scalar},
    {"exit", Type::nothing},
    {"false", Type::boolean},
    {"finishmissioninit", Type::nothing},
    {"fog", Type::scalar},
    {"fogforecast", Type::scalar},
    {"fogparams", Type::array},
    {"forcedmap", Type::array},
    {"forceend", Type::nothing},
    {"forceweatherchange", Type::nothing},
    {"freelook", Type::boolean},
    {"get3dencamera", Type::object},
    {"get3deniconsvisible", Type::array},
    {"get3denlinesvisible", Type::array},
    {"get3denmouseover", Type::array},
    {"getartillerycomputersettings", Type::array},
    {"getaudiooptionvolumes", Type::array},
    {"getcalculateplayervisibilitybyfriendly", Type::boolean},
    {"getclientstate", Type::string},
    {"getclientstatenumber", Type::string},
    {"getcursorobjectparams", Type::array},
    {"getdlcassetsusage", Type::array},
    {"getelevationoffset", Type::scalar},
    {"getlighting", Type::array},
    {"getloadedmodsinfo", Type::array},
    {"getmissiondlcs", Type::array},
    {"getmissionlayers", Type::array},
    {"getmouseposition", Type::array},
    {"getmusicplayedtime", Type::scalar},
    {"getobjectviewdistance", Type::array},
    {"getpipviewdistance", Type::scalar},
    {"getremotesensorsdisabled", Type::boolean},
    {"getresolution", Type::array},
    {"getshadowdistance", Type::scalar},
    {"getsteamfriendsservers", Type::array},
    {"getsubtitleoptions", Type::array},
    {"getterraingrid", Type::scalar},
    {"gettotaldlcusagetime", Type::scalar},
    {"groupiconselectable", Type::boolean},
    {"groupiconsvisible", Type::array},
    {"grpnull", Type::group},
    {"gusts", Type::scalar},
    {"halt", Type::nothing},
    {"hasinterface", Type::boolean},
    {"hcshownbar", Type::boolean},
    {"hudmovementlevels", Type::array},
    {"humidity", Type::scalar},
    {"independent", Type::side},
    {"initambientlife", Type::nothing},
    {"is3den", Type::boolean},
    {"is3denmultiplayer", Type::boolean},
    {"is3denpreview", Type::boolean},
    {"isactionmenuvisible", Type::boolean},
    {"isautotest", Type::boolean},
    {"isdedicated", Type::boolean},
    {"isfilepatchingenabled", Type::boolean},
    {"isgamefocused", Type::boolean},
    {"isgamepaused", Type::boolean},
    {"isinstructorfigureenabled", Type::boolean},
    {"ismultiplayer", Type::boolean},
    {"ismultiplayersolo", Type::boolean},
    {"ispipenabled", Type::boolean},
    {"isremoteexecuted", Type::boolean},
    {"isremoteexecutedjip", Type::boolean},
    {"issaving", Type::boolean},
    {"isserver", Type::boolean},
    {"issteammission", Type::boolean},
    {"isstreamfriendlyuienabled", Type::boolean},
    {"isstressdamageenabled", Type::boolean},
    {"istuthintsenabled", Type::boolean},
    {"isuicontext", Type::boolean},
    {"language", Type::string},
    {"librarycredits", Type::array},
    {"librarydisclaimers", Type::array},
    {"lightnings", Type::scalar},
    {"linebreak", Type::text},
    {"loadgame", Type::nothing},
    {"localnamespace", Type::name_space},
    {"locationnull", Type::location},
    {"logentities", Type::nothing},
    {"mapanimclear", Type::nothing},
    {"mapanimcommit", Type::nothing},
    {"mapanimdone", Type::boolean},
    {"markasfinishedonsteam", Type::boolean},
    {"missionconfigfile", Type::config},
    {"missiondifficulty", Type::scalar},
    {"missionend", Type::array},
    {"missionname", Type::string},
    {"missionnamesource", Type::string},
    {"missionnamespace", Type::name_space},
    {"missionstart", Type::array},
    {"missionversion", Type::scalar},
    {"moonintensity", Type::scalar},
    {"musicvolume", Type::scalar},
    {"netobjnull", Type::netobject},
    {"nextweatherchange", Type::scalar},
    {"nil", Type::any},
    {"objnull", Type::object},
    {"opencuratorinterface", Type::nothing},
    {"opfor", Type::side},
    {"overcast", Type::scalar},
    {"overcastforecast", Type::scalar},
    {"parsingnamespace", Type::name_space},
    {"particlesquality", Type::scalar},
    {"pi", Type::scalar},
    {"pixelgrid", Type::scalar},
    {"pixelgridbase", Type::scalar},
    {"pixelgridnouiscale", Type::scalar},
    {"pixelh", Type::scalar},
    {"pixelw", Type::scalar},
    {"playableunits", Type::array},
    {"player", Type::object},
    {"playerrespawntime", Type::scalar},
    {"playerside", Type::side},
    {"productversion", Type::array},
    {"profilename", Type::string},
    {"profilenamespace", Type::name_space},
    {"profilenamesteam", Type::string},
    {"radioenabled", Type::boolean},
    {"radiovolume", Type::scalar},
    {"rain", Type::scalar},
    {"rainbow", Type::scalar},
    {"rainparams", Type::array},
    {"remoteexecutedowner", Type::scalar},
    {"resetcamshake", Type::nothing},
    {"resistance", Type::side},
    {"reversedmousey", Type::boolean},
    {"runinitscript", Type::nothing},
    {"safezoneh", Type::scalar},
    {"safezonew", Type::scalar},
    {"safezonewabs", Type::scalar},
    {"safezonex", Type::scalar},
    {"safezonexabs", Type::scalar},
    {"safezoney", Type::scalar},
    {"savegame", Type::nothing},
    {"savejoysticks", Type::nothing},
    {"saveprofilenamespace", Type::nothing},
    {"savingenabled", Type::boolean},
    {"scriptnull", Type::script},
    {"selectnoplayer", Type::nothing},
    {"sentencesenabled", Type::boolean},
    {"servername", Type::string},
    {"servernamespace", Type::name_space},
    {"servertime", Type::scalar},
    {"shownartillerycomputer", Type::boolean},
    {"shownchat", Type::boolean},
    {"showncompass", Type::boolean},
    {"showncuratorcompass", Type::boolean},
    {"showngps", Type::boolean},
    {"shownhud", Type::array},
    {"shownmap", Type::boolean},
    {"shownpad", Type::boolean},
    {"shownradio", Type::boolean},
    {"shownscoretable", Type::scalar},
    {"shownsubtitles", Type::boolean},
    {"shownuavfeed", Type::boolean},
    {"shownwarrant", Type::boolean},
    {"shownwatch", Type::boolean},
    {"sideambientlife", Type::side},
    {"sideempty", Type::side},
    {"sideenemy", Type::side},
    {"sidefriendly", Type::side},
    {"sidelogic", Type::side},
    {"sideunknown", Type::side},
    {"simulweathersync", Type::nothing},
    {"slingloadassistantshown", Type::boolean},
    {"soundvolume", Type::scalar},
    {"speechvolume", Type::scalar},
    {"sunormoon", Type::scalar},
    {"switchableunits", Type::array},
    {"systemofunits", Type::scalar},
    {"systemtime", Type::array},
    {"systemtimeutc", Type::array},
    {"tasknull", Type::task},
    {"teammembernull", Type::team_member},
    {"teams", Type::array},
    {"teamswitch", Type::nothing},
    {"teamswitchenabled", Type::boolean},
    {"time", Type::scalar},
    {"timemultiplier", Type::scalar},
    {"true", Type::boolean},
    {"uinamespace", Type::name_space},
    {"userinputdisabled", Type::boolean},
    {"vehicles", Type::array},
    {"viewdistance", Type::scalar},
    {"visiblecompass", Type::boolean},
    {"visiblegps", Type::boolean},
    {"visiblemap", Type::boolean},
    {"visiblescoretable", Type::boolean},
    {"visiblewatch", Type::boolean},
    {"waves", Type::scalar},
    {"west", Type::side},
    {"wind", Type::array},
    {"winddir", Type::scalar},
    {"windrtd", Type::array},
    {"windstr", Type::scalar},
    {"worldname", Type::string},
    {"worldsize", Type::scalar},
};

}