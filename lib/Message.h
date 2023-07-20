#pragma once

enum CommandMessage { NEW=100, CREATE, JOIN, IN_LOBBY, LEAVE, READY, START };
enum GameMessage { NIGHT_ACTION=200, DAY_ACTION, VOTE, CHAT };