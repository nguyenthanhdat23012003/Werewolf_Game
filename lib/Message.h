#pragma once

enum CommandMessage { NEW=100, CREATE, JOIN, LEAVE, READY, START, IN_LOBBY, IN_GAME };
enum GameMessage { NIGHT_ACTION=200, DAY_ACTION, VOTE, CHAT };