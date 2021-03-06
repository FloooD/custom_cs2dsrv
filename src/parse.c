/*
 * parse.c
 *
 *  Created on: Mar 29, 2011
 *      Author: leegao
 */

#include "parse.h"



// p_xxx functions starts here.


int p_banip(char* text, int* consumed){
	char* ip; int duration = -1;  // declarations
	char* check = tokenize(text, consumed, "\x1" "si", &ip, &duration);
	if (!check) return 0;

	return banip_cmd(ip, duration);
}

int p_banname(char* text, int* consumed){
	char* name; int duration = -1;  // declarations
	char* check = tokenize(text, consumed, "\x1" "si", &name, &duration);
	if (!check) return 0;

	return banname_cmd(name, duration);
}

int p_bans(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bans_cmd();
}

int p_banusgn(char* text, int* consumed){
	int usgn; int duration = -1;  // declarations
	char* check = tokenize(text, consumed, "\x1" "ii", &usgn, &duration);
	if (!check) return 0;

	return banusgn_cmd(usgn, duration);
}

int p_changelevel(char* text, int* consumed){
	char* map;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &map);
	if (!check) return 0;

	return changelevel_cmd(map);
}

int p_changemap(char* text, int* consumed){
	char* map;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &map);
	if (!check) return 0;

	return changemap_cmd(map);
}

int p_customkill(char* text, int* consumed){
	int killer; char* weapon; int victim;  // declarations
	char* check = tokenize(text, consumed, "\x3" "isi", &killer, &weapon, &victim);
	if (!check) return 0;

	return customkill_cmd(killer, weapon, victim);
}

int p_deathslap(char* text, int* consumed){
	char* player;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &player);
	if (!check) return 0;

	return deathslap_cmd(player);
}

int p_bot_add(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_add_cmd();
}

int p_bot_add_ct(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_add_ct_cmd();
}

int p_bot_add_t(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_add_t_cmd();
}

int p_bot_count(char* text, int* consumed){
	int count;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &count);
	if (!check) return 0;

	return bot_count_cmd(count);
}

int p_bot_jointeam(char* text, int* consumed){
	int team;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &team);
	if (!check) return 0;

	return bot_jointeam_cmd(team);
}

int p_bot_kill(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_kill_cmd();
}

int p_bot_prefix(char* text, int* consumed){
	char* prefix;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &prefix);
	if (!check) return 0;

	return bot_prefix_cmd(prefix);
}

int p_bot_remove(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_remove_cmd();
}

int p_bot_remove_all(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_remove_all_cmd();
}

int p_bot_remove_ct(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_remove_ct_cmd();
}

int p_bot_remove_t(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return bot_remove_t_cmd();
}

int p_bot_skill(char* text, int* consumed){
	int skill;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &skill);
	if (!check) return 0;

	return bot_skill_cmd(skill);
}

int p_bot_weapons(char* text, int* consumed){
	int weapons;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &weapons);
	if (!check) return 0;

	return bot_weapons_cmd(weapons);
}

int p_effect(char* text, int* consumed){
	char* effect; int x; int y; int p1; int p2; int r; int g; int b;  // declarations
	char* check = tokenize(text, consumed, "\x8" "siiiiiii", &effect, &x, &y, &p1, &p2, &r, &g, &b);
	if (!check) return 0;

	return effect_cmd(effect, x, y, p1, p2, r, g, b);
}

int p_equip(char* text, int* consumed){
	int player; int weapon;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &weapon);
	if (!check) return 0;

	return equip_cmd(player, weapon);
}

int p_explosion(char* text, int* consumed){
	int x; int y; int size; int damage; int source;  // declarations
	char* check = tokenize(text, consumed, "\x5" "iiiii", &x, &y, &size, &damage, &source);
	if (!check) return 0;

	return explosion_cmd(x, y, size, damage, source);
}

int p_flashplayer(char* text, int* consumed){
	int player; int intensity;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &intensity);
	if (!check) return 0;

	return flashplayer_cmd(player, intensity);
}

int p_flashposition(char* text, int* consumed){
	int x; int y; int intensity;  // declarations
	char* check = tokenize(text, consumed, "\x3" "iii", &x, &y, &intensity);
	if (!check) return 0;

	return flashposition_cmd(x, y, intensity);
}

int p_hudtxt(char* text, int* consumed){
	int id; char* txt; int x; int y; int align;  // declarations
	char* check = tokenize(text, consumed, "\x5" "isiii", &id, &txt, &x, &y, &align);
	if (!check) return 0;

	return hudtxt_cmd(id, txt, x, y, align);
}

int p_hudtxt2(char* text, int* consumed){
	int player; int id; char* txt; int x; int y; int align;  // declarations
	char* check = tokenize(text, consumed, "\x6" "iisiii", &player, &id, &txt, &x, &y, &align);
	if (!check) return 0;

	return hudtxt2_cmd(player, id, txt, x, y, align);
}

int p_hudtxtalphafade(char* text, int* consumed){
	int player; int id; int duration; float alpha;  // declarations
	char* check = tokenize(text, consumed, "\x4" "iiif", &player, &id, &duration, &alpha);
	if (!check) return 0;

	return hudtxtalphafade_cmd(player, id, duration, alpha);
}

int p_hudtxtcolorfade(char* text, int* consumed){
	int player; int id; int duration; int red; int green; int blue;  // declarations
	char* check = tokenize(text, consumed, "\x6" "iiiiii", &player, &id, &duration, &red, &green, &blue);
	if (!check) return 0;

	return hudtxtcolorfade_cmd(player, id, duration, red, green, blue);
}

int p_hudtxtmove(char* text, int* consumed){
	int player; int id; int duration; int x; int y;  // declarations
	char* check = tokenize(text, consumed, "\x5" "iiiii", &player, &id, &duration, &x, &y);
	if (!check) return 0;

	return hudtxtmove_cmd(player, id, duration, x, y);
}

int p_items(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return items_cmd();
}

int p_kick(char* text, int* consumed){
	char* player; char* reason;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ss", &player, &reason);
	if (!check) return 0;

	return kick_cmd(player, reason);
}

int p_killobject(char* text, int* consumed){
	int object;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &object);
	if (!check) return 0;

	return killobject_cmd(object);
}

int p_killplayer(char* text, int* consumed){
	int player;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &player);
	if (!check) return 0;

	return killplayer_cmd(player);
}

int p_listbans(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return listbans_cmd();
}

int p_listplayers(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return listplayers_cmd();
}

int p_logaddress_add(char* text, int* consumed){
	char* address;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &address);
	if (!check) return 0;

	return logaddress_add_cmd(address);
}

int p_logaddress_remove(char* text, int* consumed){
	char* address;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &address);
	if (!check) return 0;

	return logaddress_remove_cmd(address);
}

int p_logaddress_removeall(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return logaddress_removeall_cmd();
}

int p_lua(char* text, int* consumed){
	char* script;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &script);
	if (!check) return 0;

	return lua_cmd(script);
}

int p_luareset(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return luareset_cmd();
}

int p_makect(char* text, int* consumed){
	char* player;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &player);
	if (!check) return 0;

	return makect_cmd(player);
}

int p_makespec(char* text, int* consumed){
	char* player;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &player);
	if (!check) return 0;

	return makespec_cmd(player);
}

int p_maket(char* text, int* consumed){
	char* player;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &player);
	if (!check) return 0;

	return maket_cmd(player);
}

int p_map(char* text, int* consumed){
	char* map;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &map);
	if (!check) return 0;

	return map_cmd(map);
}

int p_maps(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return maps_cmd();
}

int p_mp_antispeeder(char* text, int* consumed){
	int antispeeder;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &antispeeder);
	if (!check) return 0;

	return mp_antispeeder_cmd(antispeeder);
}

int p_mp_autogamemode(char* text, int* consumed){
	int autogamemode;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &autogamemode);
	if (!check) return 0;

	return mp_autogamemode_cmd(autogamemode);
}

int p_mp_autoteambalance(char* text, int* consumed){
	int balance;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &balance);
	if (!check) return 0;

	return mp_autoteambalance_cmd(balance);
}

int p_mp_building_health(char* text, int* consumed){
	char* building; int price;  // declarations
	char* check = tokenize(text, consumed, "\x2" "si", &building, &price);
	if (!check) return 0;

	return mp_building_health_cmd(building, price);
}

int p_mp_building_limit(char* text, int* consumed){
	char* building; int limit;  // declarations
	char* check = tokenize(text, consumed, "\x2" "si", &building, &limit);
	if (!check) return 0;

	return mp_building_limit_cmd(building, limit);
}

int p_mp_building_price(char* text, int* consumed){
	char* building; int price;  // declarations
	char* check = tokenize(text, consumed, "\x2" "si", &building, &price);
	if (!check) return 0;

	return mp_building_price_cmd(building, price);
}

int p_mp_buytime(char* text, int* consumed){
	float time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &time);
	if (!check) return 0;

	return mp_buytime_cmd(time);
}

int p_mp_c4timer(char* text, int* consumed){
	int time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &time);
	if (!check) return 0;

	return mp_c4timer_cmd(time);
}

int p_mp_curtailedexplosions(char* text, int* consumed){
	int curtailed;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &curtailed);
	if (!check) return 0;

	return mp_curtailedexplosions_cmd(curtailed);
}

int p_mp_damagefactor(char* text, int* consumed){
	float factor;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &factor);
	if (!check) return 0;

	return mp_damagefactor_cmd(factor);
}

int p_mp_deathdrop(char* text, int* consumed){
	int drop;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &drop);
	if (!check) return 0;

	return mp_deathdrop_cmd(drop);
}

int p_mp_dispenser_health(char* text, int* consumed){
	int health;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &health);
	if (!check) return 0;

	return mp_dispenser_health_cmd(health);
}

int p_mp_dispenser_money(char* text, int* consumed){
	int money;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &money);
	if (!check) return 0;

	return mp_dispenser_money_cmd(money);
}

int p_mp_dmspawnmoney(char* text, int* consumed){
	int money;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &money);
	if (!check) return 0;

	return mp_dmspawnmoney_cmd(money);
}

int p_mp_dropgrenades(char* text, int* consumed){
	int drop;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &drop);
	if (!check) return 0;

	return mp_dropgrenades_cmd(drop);
}

int p_mp_floodprot(char* text, int* consumed){
	int flood;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &flood);
	if (!check) return 0;

	return mp_floodprot_cmd(flood);
}

int p_mp_freezetime(char* text, int* consumed){
	int time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &time);
	if (!check) return 0;

	return mp_freezetime_cmd(time);
}

int p_mp_grenaderebuy(char* text, int* consumed){
	int allowed;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &allowed);
	if (!check) return 0;

	return mp_grenaderebuy_cmd(allowed);
}

int p_mp_hostagepenalty(char* text, int* consumed){
	int kills;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &kills);
	if (!check) return 0;

	return mp_hostagepenalty_cmd(kills);
}

int p_mp_idleaction(char* text, int* consumed){
	int action;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &action);
	if (!check) return 0;

	return mp_idleaction_cmd(action);
}

int p_mp_idlekick(char* text, int* consumed){
	int idle;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &idle);
	if (!check) return 0;

	return mp_idlekick_cmd(idle);
}

int p_mp_idletime(char* text, int* consumed){
	int time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &time);
	if (!check) return 0;

	return mp_idletime_cmd(time);
}

int p_mp_infammo(char* text, int* consumed){
	int infinite;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &infinite);
	if (!check) return 0;

	return mp_infammo_cmd(infinite);
}

int p_mp_kevlar(char* text, int* consumed){
	float percent;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &percent);
	if (!check) return 0;

	return mp_kevlar_cmd(percent);
}

int p_mp_kickpercent(char* text, int* consumed){
	float percent;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &percent);
	if (!check) return 0;

	return mp_kickpercent_cmd(percent);
}

int p_mp_killbuildingmoney(char* text, int* consumed){
	int money;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &money);
	if (!check) return 0;

	return mp_killbuildingmoney_cmd(money);
}

int p_mp_killbuildings(char* text, int* consumed){
	int kill;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &kill);
	if (!check) return 0;

	return mp_killbuildings_cmd(kill);
}

int p_mp_killinfo(char* text, int* consumed){
	int info;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &info);
	if (!check) return 0;

	return mp_killinfo_cmd(info);
}

int p_mp_killteambuildings(char* text, int* consumed){
	int mode;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &mode);
	if (!check) return 0;

	return mp_killteambuildings_cmd(mode);
}

int p_mp_lagcompensation(char* text, int* consumed){
	int max;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &max);
	if (!check) return 0;

	return mp_lagcompensation_cmd(max);
}

int p_mp_localrconoutput(char* text, int* consumed){
	int output;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &output);
	if (!check) return 0;

	return mp_localrconoutput_cmd(output);
}

int p_mp_luamap(char* text, int* consumed){
	int use;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &use);
	if (!check) return 0;

	return mp_luamap_cmd(use);
}

int p_mp_luaserver(char* text, int* consumed){
	char* file;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &file);
	if (!check) return 0;

	return mp_luaserver_cmd(file);
}

int p_mp_mapgoalscore(char* text, int* consumed){
	int score;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &score);
	if (!check) return 0;

	return mp_mapgoalscore_cmd(score);
}

int p_mp_mapvoteratio(char* text, int* consumed){
	float percent;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &percent);
	if (!check) return 0;

	return mp_mapvoteratio_cmd(percent);
}

int p_mp_maxclientsip(char* text, int* consumed){
	int clients;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &clients);
	if (!check) return 0;

	return mp_maxclientsip_cmd(clients);
}

int p_mp_maxrconfails(char* text, int* consumed){
	int max;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &max);
	if (!check) return 0;

	return mp_maxrconfails_cmd(max);
}

int p_mp_natholepunching(char* text, int* consumed){
	int allow;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &allow);
	if (!check) return 0;

	return mp_natholepunching_cmd(allow);
}

int p_mp_pinglimit(char* text, int* consumed){
	int limit;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &limit);
	if (!check) return 0;

	return mp_pinglimit_cmd(limit);
}

int p_mp_postspawn(char* text, int* consumed){
	int time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &time);
	if (!check) return 0;

	return mp_postspawn_cmd(time);
}

int p_mp_radar(char* text, int* consumed){
	int radar;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &radar);
	if (!check) return 0;

	return mp_radar_cmd(radar);
}

int p_mp_randomspawn(char* text, int* consumed){
	int random;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &random);
	if (!check) return 0;

	return mp_randomspawn_cmd(random);
}

int p_mp_reservations(char* text, int* consumed){
	char* reservations;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &reservations);
	if (!check) return 0;

	return mp_reservations_cmd(reservations);
}

int p_mp_respawndelay(char* text, int* consumed){
	int delay;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &delay);
	if (!check) return 0;

	return mp_respawndelay_cmd(delay);
}

int p_mp_roundlimit(char* text, int* consumed){
	int rounds;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &rounds);
	if (!check) return 0;

	return mp_roundlimit_cmd(rounds);
}

int p_mp_roundtime(char* text, int* consumed){
	float time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &time);
	if (!check) return 0;

	return mp_roundtime_cmd(time);
}

int p_mp_startmoney(char* text, int* consumed){
	int money;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &money);
	if (!check) return 0;

	return mp_startmoney_cmd(money);
}

int p_mp_supply_items(char* text, int* consumed){
	char* itemlist;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &itemlist);
	if (!check) return 0;

	return mp_supply_items_cmd(itemlist);
}

int p_mp_teamkillpenalty(char* text, int* consumed){
	int kills;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &kills);
	if (!check) return 0;

	return mp_teamkillpenalty_cmd(kills);
}

int p_mp_teleportreload(char* text, int* consumed){
	int reload;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &reload);
	if (!check) return 0;

	return mp_teleportreload_cmd(reload);
}

int p_mp_tempbantime(char* text, int* consumed){
	float time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &time);
	if (!check) return 0;

	return mp_tempbantime_cmd(time);
}

int p_mp_timelimit(char* text, int* consumed){
	float time;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &time);
	if (!check) return 0;

	return mp_timelimit_cmd(time);
}

int p_mp_tkpunish(char* text, int* consumed){
	int kill;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &kill);
	if (!check) return 0;

	return mp_tkpunish_cmd(kill);
}

int p_mp_trace(char* text, int* consumed){
	int accuracy;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &accuracy);
	if (!check) return 0;

	return mp_trace_cmd(accuracy);
}

int p_mp_turretdamage(char* text, int* consumed){
	int damage;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &damage);
	if (!check) return 0;

	return mp_turretdamage_cmd(damage);
}

int p_mp_unbuildable(char* text, int* consumed){
	char* buildings;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &buildings);
	if (!check) return 0;

	return mp_unbuildable_cmd(buildings);
}

int p_mp_unbuyable(char* text, int* consumed){
	char* unbuyable;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &unbuyable);
	if (!check) return 0;

	return mp_unbuyable_cmd(unbuyable);
}

int p_mp_vulnerablehostages(char* text, int* consumed){
	int vulnerable;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &vulnerable);
	if (!check) return 0;

	return mp_vulnerablehostages_cmd(vulnerable);
}

int p_mp_winlimit(char* text, int* consumed){
	int wins;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &wins);
	if (!check) return 0;

	return mp_winlimit_cmd(wins);
}

int p_mp_wpndmg(char* text, int* consumed){
	char* name; int damage;  // declarations
	char* check = tokenize(text, consumed, "\x2" "si", &name, &damage);
	if (!check) return 0;

	return mp_wpndmg_cmd(name, damage);
}

int p_mp_wpndmg_z1(char* text, int* consumed){
	char* name; int damage;  // declarations
	char* check = tokenize(text, consumed, "\x2" "si", &name, &damage);
	if (!check) return 0;

	return mp_wpndmg_z1_cmd(name, damage);
}

int p_mp_wpndmg_z2(char* text, int* consumed){
	char* name; int damage;  // declarations
	char* check = tokenize(text, consumed, "\x2" "si", &name, &damage);
	if (!check) return 0;

	return mp_wpndmg_z2_cmd(name, damage);
}

int p_mp_zombiedmg(char* text, int* consumed){
	float percentage;  // declarations
	char* check = tokenize(text, consumed, "\x1" "f", &percentage);
	if (!check) return 0;

	return mp_zombiedmg_cmd(percentage);
}

int p_mp_zombiekillequip(char* text, int* consumed){
	int weapon;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &weapon);
	if (!check) return 0;

	return mp_zombiekillequip_cmd(weapon);
}

int p_mp_zombiekillscore(char* text, int* consumed){
	int kill;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &kill);
	if (!check) return 0;

	return mp_zombiekillscore_cmd(kill);
}

int p_mp_zombierecover(char* text, int* consumed){
	int recover;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &recover);
	if (!check) return 0;

	return mp_zombierecover_cmd(recover);
}

int p_mp_zombiespeedmod(char* text, int* consumed){
	int speedmod;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &speedmod);
	if (!check) return 0;

	return mp_zombiespeedmod_cmd(speedmod);
}

int p_msg(char* text, int* consumed){
	char* message;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &message);
	if (!check) return 0;

	return msg_cmd(message);
}

int p_rcon(char* text, int* consumed){
	char* commands;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &commands);
	if (!check) return 0;

	return rcon_cmd(commands);
}

int p_rcon_password(char* text, int* consumed){
	char* password;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &password);
	if (!check) return 0;

	return rcon_password_cmd(password);
}

int p_rcon_pw(char* text, int* consumed){
	char* password;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &password);
	if (!check) return 0;

	return rcon_pw_cmd(password);
}

int p_removeitem(char* text, int* consumed){
	int item;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &item);
	if (!check) return 0;

	return removeitem_cmd(item);
}

int p_reroute(char* text, int* consumed){
	int player; char* address;  // declarations
	char* check = tokenize(text, consumed, "\x2" "is", &player, &address);
	if (!check) return 0;

	return reroute_cmd(player, address);
}

int p_restart(char* text, int* consumed){
	int delay;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &delay);
	if (!check) return 0;

	return restart_cmd(delay);
}

int p_restartround(char* text, int* consumed){
	int delay;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &delay);
	if (!check) return 0;

	return restartround_cmd(delay);
}

int p_setarmor(char* text, int* consumed){
	int player; int armor;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &armor);
	if (!check) return 0;

	return setarmor_cmd(player, armor);
}

int p_setdeaths(char* text, int* consumed){
	int player; int deaths;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &deaths);
	if (!check) return 0;

	return setdeaths_cmd(player, deaths);
}

int p_sethealth(char* text, int* consumed){
	int player; int health;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &health);
	if (!check) return 0;

	return sethealth_cmd(player, health);
}

int p_setmaxhealth(char* text, int* consumed){
	int player; int maxhealth;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &maxhealth);
	if (!check) return 0;

	return setmaxhealth_cmd(player, maxhealth);
}

int p_setmoney(char* text, int* consumed){
	int player; int money;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &money);
	if (!check) return 0;

	return setmoney_cmd(player, money);
}

int p_setname(char* text, int* consumed){
	int player; char* name;  // declarations
	char* check = tokenize(text, consumed, "\x2" "is", &player, &name);
	if (!check) return 0;

	return setname_cmd(player, name);
}

int p_setpos(char* text, int* consumed){
	int player; int x; int y;  // declarations
	char* check = tokenize(text, consumed, "\x3" "iii", &player, &x, &y);
	if (!check) return 0;

	return setpos_cmd(player, x, y);
}

int p_setscore(char* text, int* consumed){
	int player; int score;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &score);
	if (!check) return 0;

	return setscore_cmd(player, score);
}

int p_setweapon(char* text, int* consumed){
	int player; int weapon;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &weapon);
	if (!check) return 0;

	return setweapon_cmd(player, weapon);
}

int p_shake(char* text, int* consumed){
	int player; int power;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &power);
	if (!check) return 0;

	return shake_cmd(player, power);
}

int p_slap(char* text, int* consumed){
	char* player;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &player);
	if (!check) return 0;

	return slap_cmd(player);
}

int p_spawnitem(char* text, int* consumed){
	int item; int x; int y;  // declarations
	char* check = tokenize(text, consumed, "\x3" "iii", &item, &x, &y);
	if (!check) return 0;

	return spawnitem_cmd(item, x, y);
}

int p_spawnnpc(char* text, int* consumed){
	int type; int x; int y; int rot;  // declarations
	char* check = tokenize(text, consumed, "\x4" "iiii", &type, &x, &y, &rot);
	if (!check) return 0;

	return spawnnpc_cmd(type, x, y, rot);
}

int p_spawnobject(char* text, int* consumed){
	int type; int x; int y; int rot; int mode; int team; int player;  // declarations
	char* check = tokenize(text, consumed, "\x7" "iiiiiii", &type, &x, &y, &rot, &mode, &team, &player);
	if (!check) return 0;

	return spawnobject_cmd(type, x, y, rot, mode, team, player);
}

int p_spawnplayer(char* text, int* consumed){
	int player; int x; int y;  // declarations
	char* check = tokenize(text, consumed, "\x3" "iii", &player, &x, &y);
	if (!check) return 0;

	return spawnplayer_cmd(player, x, y);
}

int p_speedmod(char* text, int* consumed){
	int player; int speedmod;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &speedmod);
	if (!check) return 0;

	return speedmod_cmd(player, speedmod);
}

int p_strip(char* text, int* consumed){
	int player; int weapon;  // declarations
	char* check = tokenize(text, consumed, "\x2" "ii", &player, &weapon);
	if (!check) return 0;

	return strip_cmd(player, weapon);
}

int p_sv_fow(char* text, int* consumed){
	int fow;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &fow);
	if (!check) return 0;

	return sv_fow_cmd(fow);
}

int p_sv_friendlyfire(char* text, int* consumed){
	int ff;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &ff);
	if (!check) return 0;

	return sv_friendlyfire_cmd(ff);
}

int p_sv_gamemode(char* text, int* consumed){
	int mode;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &mode);
	if (!check) return 0;

	return sv_gamemode_cmd(mode);
}

int p_sv_gm(char* text, int* consumed){
	int mode;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &mode);
	if (!check) return 0;

	return sv_gm_cmd(mode);
}

int p_sv_hostport(char* text, int* consumed){
	int port;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &port);
	if (!check) return 0;

	return sv_hostport_cmd(port);
}

int p_sv_lan(char* text, int* consumed){
	int lan;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &lan);
	if (!check) return 0;

	return sv_lan_cmd(lan);
}

int p_sv_map(char* text, int* consumed){
	char* map;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &map);
	if (!check) return 0;

	return sv_map_cmd(map);
}

int p_sv_maptransfer(char* text, int* consumed){
	int transfer;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &transfer);
	if (!check) return 0;

	return sv_maptransfer_cmd(transfer);
}

int p_sv_maxplayers(char* text, int* consumed){
	int players;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &players);
	if (!check) return 0;

	return sv_maxplayers_cmd(players);
}

int p_sv_msg(char* text, int* consumed){
	char* message;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &message);
	if (!check) return 0;

	return sv_msg_cmd(message);
}

int p_sv_msg2(char* text, int* consumed){
	int player; char* message;  // declarations
	char* check = tokenize(text, consumed, "\x2" "is", &player, &message);
	if (!check) return 0;

	return sv_msg2_cmd(player, message);
}

int p_sv_name(char* text, int* consumed){
	char* name;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &name);
	if (!check) return 0;

	return sv_name_cmd(name);
}

int p_sv_password(char* text, int* consumed){
	char* password;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &password);
	if (!check) return 0;

	return sv_password_cmd(password);
}

int p_sv_rcon(char* text, int* consumed){
	char* rcon;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &rcon);
	if (!check) return 0;

	return sv_rcon_cmd(rcon);
}

int p_sv_restart(char* text, int* consumed){
	int delay;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &delay);
	if (!check) return 0;

	return sv_restart_cmd(delay);
}

int p_sv_restartround(char* text, int* consumed){
	int delay;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &delay);
	if (!check) return 0;

	return sv_restartround_cmd(delay);
}

int p_sv_sound(char* text, int* consumed){
	char* soundfile;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &soundfile);
	if (!check) return 0;

	return sv_sound_cmd(soundfile);
}

int p_sv_sound2(char* text, int* consumed){
	int player; char* soundfile;  // declarations
	char* check = tokenize(text, consumed, "\x2" "is", &player, &soundfile);
	if (!check) return 0;

	return sv_sound2_cmd(player, soundfile);
}

int p_sv_specmode(char* text, int* consumed){
	int mode;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &mode);
	if (!check) return 0;

	return sv_specmode_cmd(mode);
}

int p_sv_spraytransfer(char* text, int* consumed){
	int spray;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &spray);
	if (!check) return 0;

	return sv_spraytransfer_cmd(spray);
}

int p_sv_usgnonly(char* text, int* consumed){
	int usgn;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &usgn);
	if (!check) return 0;

	return sv_usgnonly_cmd(usgn);
}

int p_transfer_speed(char* text, int* consumed){
	int speed;  // declarations
	char* check = tokenize(text, consumed, "\x1" "i", &speed);
	if (!check) return 0;

	return transfer_speed_cmd(speed);
}

int p_trigger(char* text, int* consumed){
	char* trigger;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &trigger);
	if (!check) return 0;

	return trigger_cmd(trigger);
}

int p_unban(char* text, int* consumed){
	char* banmask;  // declarations
	char* check = tokenize(text, consumed, "\x1" "s", &banmask);
	if (!check) return 0;

	return unban_cmd(banmask);
}

int p_unbanall(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return unbanall_cmd();
}

int p_usgn_addserver(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return usgn_addserver_cmd();
}

int p_usgn_info(char* text, int* consumed){
	 // declarations
	char* check = tokenize(text, consumed, "\x0" "");
	if (!check) return 0;

	return usgn_info_cmd();
}


// p_xxx ends here




/* tokenizer splits the cmd into the args, returns the cmd-type
 *
	int c;char* s;float port;
	char* cmd = tokenize("  banip \"192.168.1.1\" 3.14",&c,"sf",&s,&port);
	printf("cmd: %s, ip:%s, port:%f, consumed:%d\n",cmd, s,port,c);
 *
 * fmt = s i f
 */
char* tokenize(char* text, int* consumed, char* fmt, ...){
	int min = *(fmt++);
	int i,j, quot = 0, nargs = strlen(fmt), term = 0;
	*consumed = 0;
	char* cmd = NULL;
	va_list args;
	va_start(args, fmt);

	for (i=-1;i<nargs;i++){
		char* buf = NULL;
		if ((!*text || term) && i>=min) {
			va_end(args);
			return cmd;
		}else if (!*text || term){
			printf("[Parse] Too few arguments.\n");
			return NULL;
		}

		while(*text && (*text == ' ' || *text == ';')){
			text++;(*consumed)++;
		}
		if (*text == '"'){
			quot=1;text++;(*consumed)++;
		}
		for(j=0;text[j];j++){
			// unquoted termination condition
			if (!quot && (text[j] == ' ' || text[j] == ';' || !text[j])){
				if (text[j] == ';') term = 1;
				buf = (char*)malloc(j+1);
				memcpy(buf, text, j);
				buf[j]='\0';
				while(text[j] == ' ' || text[j] == ';') j++;
				text+=j;
				break;
			// quoted termination condition
			} else if (quot && text[j] == '"'){
				buf = (char*)malloc(j+1);
				memcpy(buf, text, j);
				buf[j++]='\0';
				text+=j;
				break;
			}
		}
		if(!buf){
			buf = (char*)malloc(j);
			memcpy(buf, text, j);
			buf[j] = '\0';
			text+=j;
		}

		(*consumed)+=j;
		quot=0;
		if(i<0) {cmd=buf;continue;}
		switch(fmt[i]){
		case 's':{
			char** a = va_arg(args, char**);
			*a = buf;
			break;
		}
		case 'i':{
			int* a = va_arg(args, int*);
			sscanf(buf, "%d", a);
			break;
		}
		case 'f':{
			float* a = va_arg(args, float*);
			sscanf(buf, "%f", a);
			break;
		}
		default:
			printf("[Parse] Bad parser format '%c'\n",fmt[i]);
			va_end(args);
			return NULL;
		}

	}

	va_end(args);
	return cmd;
}

void init_parse(){
	parse_t = hashmap_new();
#define add(type) hashmap_put(parse_t, #type, &p_##type);


	add(banip);

	add(banname);

	add(bans);

	add(banusgn);

	add(changelevel);

	add(changemap);

	add(customkill);

	add(deathslap);

	add(bot_add);

	add(bot_add_ct);

	add(bot_add_t);

	add(bot_count);

	add(bot_jointeam);

	add(bot_kill);

	add(bot_prefix);

	add(bot_remove);

	add(bot_remove_all);

	add(bot_remove_ct);

	add(bot_remove_t);

	add(bot_skill);

	add(bot_weapons);

	add(effect);

	add(equip);

	add(explosion);

	add(flashplayer);

	add(flashposition);

	add(hudtxt);

	add(hudtxt2);

	add(hudtxtalphafade);

	add(hudtxtcolorfade);

	add(hudtxtmove);

	add(items);

	add(kick);

	add(killobject);

	add(killplayer);

	add(listbans);

	add(listplayers);

	add(logaddress_add);

	add(logaddress_remove);

	add(logaddress_removeall);

	add(lua);

	add(luareset);

	add(makect);

	add(makespec);

	add(maket);

	add(map);

	add(maps);

	add(mp_antispeeder);

	add(mp_autogamemode);

	add(mp_autoteambalance);

	add(mp_building_health);

	add(mp_building_limit);

	add(mp_building_price);

	add(mp_buytime);

	add(mp_c4timer);

	add(mp_curtailedexplosions);

	add(mp_damagefactor);

	add(mp_deathdrop);

	add(mp_dispenser_health);

	add(mp_dispenser_money);

	add(mp_dmspawnmoney);

	add(mp_dropgrenades);

	add(mp_floodprot);

	add(mp_freezetime);

	add(mp_grenaderebuy);

	add(mp_hostagepenalty);

	add(mp_idleaction);

	add(mp_idlekick);

	add(mp_idletime);

	add(mp_infammo);

	add(mp_kevlar);

	add(mp_kickpercent);

	add(mp_killbuildingmoney);

	add(mp_killbuildings);

	add(mp_killinfo);

	add(mp_killteambuildings);

	add(mp_lagcompensation);

	add(mp_localrconoutput);

	add(mp_luamap);

	add(mp_luaserver);

	add(mp_mapgoalscore);

	add(mp_mapvoteratio);

	add(mp_maxclientsip);

	add(mp_maxrconfails);

	add(mp_natholepunching);

	add(mp_pinglimit);

	add(mp_postspawn);

	add(mp_radar);

	add(mp_randomspawn);

	add(mp_reservations);

	add(mp_respawndelay);

	add(mp_roundlimit);

	add(mp_roundtime);

	add(mp_startmoney);

	add(mp_supply_items);

	add(mp_teamkillpenalty);

	add(mp_teleportreload);

	add(mp_tempbantime);

	add(mp_timelimit);

	add(mp_tkpunish);

	add(mp_trace);

	add(mp_turretdamage);

	add(mp_unbuildable);

	add(mp_unbuyable);

	add(mp_vulnerablehostages);

	add(mp_winlimit);

	add(mp_wpndmg);

	add(mp_wpndmg_z1);

	add(mp_wpndmg_z2);

	add(mp_zombiedmg);

	add(mp_zombiekillequip);

	add(mp_zombiekillscore);

	add(mp_zombierecover);

	add(mp_zombiespeedmod);

	add(msg);

	add(rcon);

	add(rcon_password);

	add(rcon_pw);

	add(removeitem);

	add(reroute);

	add(restart);

	add(restartround);

	add(setarmor);

	add(setdeaths);

	add(sethealth);

	add(setmaxhealth);

	add(setmoney);

	add(setname);

	add(setpos);

	add(setscore);

	add(setweapon);

	add(shake);

	add(slap);

	add(spawnitem);

	add(spawnnpc);

	add(spawnobject);

	add(spawnplayer);

	add(speedmod);

	add(strip);

	add(sv_fow);

	add(sv_friendlyfire);

	add(sv_gamemode);

	add(sv_gm);

	add(sv_hostport);

	add(sv_lan);

	add(sv_map);

	add(sv_maptransfer);

	add(sv_maxplayers);

	add(sv_msg);

	add(sv_msg2);

	add(sv_name);

	add(sv_password);

	add(sv_rcon);

	add(sv_restart);

	add(sv_restartround);

	add(sv_sound);

	add(sv_sound2);

	add(sv_specmode);

	add(sv_spraytransfer);

	add(sv_usgnonly);

	add(transfer_speed);

	add(trigger);

	add(unban);

	add(unbanall);

	add(usgn_addserver);

	add(usgn_info);

#undef add

}

void parse(char* text){
	if (!*text) return;
	int consumed;
	char* type = tokenize(text,&consumed,"\0");
	parse_h handler;
	int err = hashmap_get(parse_t, (char*)type, (void**)(&handler));
	if (err != MAP_OK){
		printf("[Parse] Invalid command '%s'\n",type);
		return parse(text+consumed);
	}

	int status = handler(text,&consumed);
	if (!status){
		// pass
	}
	parse(text+consumed);
}


