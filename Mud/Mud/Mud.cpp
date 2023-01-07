
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

// 명령어 관련 상태관리
enum PLAYER_COMMAND_STATE
{
	COMMAND_MOVE_EAST = 0,
	COMMAND_MOVE_WEST,
	COMMAND_MOVE_SOUTH,
	COMMAND_MOVE_NORTH,
	COMMAND_HELP,
	COMMAND_ATTACK,
	COMMAND_REST,
	COMMAND_POSITION,
	COMMAND_BAG,
	COMMAND_STATE,
	COMMAND_EQUIP,
	COMMAND_BUY,
	COMMAND_SELL,
	COMMAND_LOOK,
	COMMAND_WRONG,
};

// 플레이어 기본틀
struct PLAYER
{
	PLAYER_COMMAND_STATE playerCommandState;
	char name[10];		//플레이어의 이름
	int HP;				//플레이어의 체력
	int MAX_HP;			//플레이어의 최대 체력
	int MP;				//플레이어의 마력
	int MAX_MP;			//플레이어의 최대 마력
	int Gold;			//플레이어의 소지 골드
	int ATK;			//플레이어의 공격력
	int DEF;			//플레이어의 방어력
	float CriProb; 		//플레이어의 크리티컬 확률
	int PosX;			//플레이어의 위치 x
	int PosY;			//플레이어의 위치 y

	PLAYER() //변수 초기화
	{
		playerCommandState = COMMAND_WRONG;
		strcpy_s(name, "꼬부기");
		HP = 100;
		MAX_HP = 100;
		MP = 50;
		MAX_MP = 100;
		Gold = 1000;
		ATK = 10;
		DEF = 5;
		CriProb = 1.5f;
		PosX = 200;
		PosY = 200;
	}
};

PLAYER g_player;


// 몬스터 기본틀
struct MONSTER
{
	char name[20];		//이름
	char say[64];		//등장 대사
	int hp;				//체력
	int mp;				//마력
	int atk;			//공격력
	int def;			//방어력
	float criPro;		//크리티컬 확률 	
	int dropGold;		//드랍 골드
	int dropItemIndex;	//드랍 아이템
	int dropExp;		//드랍 경험치
	float itemDropPro;  //아이템 드랍 확률
};

MONSTER g_monster[100];


// 맵 기본정보틀
struct MAP_INFO
{
	int eventIndex;
	int monsterIndex;
	int npcIndex;
};

MAP_INFO g_map[500][500] = { 0, };

// 전역 변수 - 플레이어
char g_playerName[] = "꼬부기"; // 플레이어의 이름
int g_playerHP = 100;			// 플레이어의 체력
int g_playerMP = 50;			// 플레이어의 마력
int g_playerGold = 1000;		// 플레이어의 소지 골드
int g_playerATK = 10;			// 플레이어의 공격력
int g_playerDEF = 5;			// 플레이어의 방어력
int g_playerPosX = 200;			// 플레이어의 x 위치
int g_playerPosY = 200;			// 플레이어의 y 위치
float g_playerCriProb = 1.5f;	// 플레이어의 크리티컬 확률

// 입력 버퍼
char g_inputBuf[128]= "";

void title();
void prologue();
void textColor(int _colorValue);
void notice(const char* _notice);
void npcDialogue(const char* _name, const char* _dialogue);
void charDialogue(const char* _name, const char* _dialogue);
void question(const char* _question);
void fantasyLandMap();
void helpDescription(const char* _command, const char* _description);
void monsterStatusSet(int _index, const char* _name,const char* _say, int _hp, int _mp, int _atk, int _def, float _criPro, int _dropGold, int _dropItemIndex, int _dropExp, float _itemDropPro);
void monsterData();
void oneAreaSet(int _x, int _y, int _monsterIndex, int _eventIndex, int _npcIndex);
void mapInfoSetting();
void checkMapEvent();
void checkBattle();
void battleMessage(const char* _notice);
void message(const char* _notice);



int main()
{
	srand( ( unsigned ) time ( NULL ));
	monsterData();
	mapInfoSetting();
	system("title 꼬부기의 대모험");
	system("mode con:lines=50 cols=100");

	title();
	prologue();
	fantasyLandMap();
}

void title()
{
	printf("┌──────────────────────────────────────────────────────────────────────────┐\n");
	printf("│                                                                          │\n");
	printf("│");

	textColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("              꼬부기의 대모험");

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	printf(" - 부제 : 토끼의");

	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(" 간");

	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	printf("을 찾아서...              │\n");
	printf("│                                                                          │\n");
	printf("│                                     created by park minwook on 2023.01.06│\n");
	printf("└──────────────────────────────────────────────────────────────────────────┘\n");
}

void prologue()
{
	npcDialogue("상어 대신", "이보게, 상태는 어떠하신가?");
	npcDialogue("문어 의원", "가슴을 움켜쥐고 있는 이 불가사리를 떼어낼려면 천년묵은 빨간토끼의 간이 필요합니다.");
	npcDialogue("상어 대신", "토끼라면 지상에 사는 것이 아닌가? 우리가 어찌 지상으로 갈 수 있단 말인가?");
	charDialogue(g_playerName, "제가 가겠습니다. 저희 종족은 육지와 바다 모두를 다닐 수 있습니다.");
	npcDialogue("상어 대신", "그럼 이 임무를 너에게 맡기겠다. 떠나거라! 육지로...");
	charDialogue(g_playerName, "반드시 토끼의 간을 구해오겠습니다.");
	npcDialogue("???", ".....");
	npcDialogue("???", "'계획에 차질이 생기겠군. 일단 연락을 해야겠어.'");

	printf("☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★☆★\n\n");

	while (1)
	{
		question("육지로 올라가시겠습니까? <예 or 아니오> :");

		if (strcmp(g_inputBuf, "예") == 0)
		{
			charDialogue(g_playerName, "휴~~, 드디어 도착했군. 육지도 오랜만인걸?");
			charDialogue(g_playerName, "그런데 빨간털을 가진 토끼라... 흰토끼는 많이 봤지만 빨간토끼는 어디 있는거지? 일단 흰토끼를 찾아서 정보를 모아야겠다.");
			break;
		}
		else if (strcmp(g_inputBuf, "아니오") == 0)
		{
			printf("%s은(는) 바다에서 계속 헤엄을 치며 놀고 있습니다.(임무를 수행해야지! ㅡ.ㅡ;)\n", g_playerName);
		}
		else
		{
			printf("%s은(는) 장난을 치고 있습니다.\n\n", g_player.name);
		}
	}
}


// 텍스트 색깔을 바꿔주는 함수
void textColor(int _colorvalue)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _colorvalue);
}


void npcDialogue(const char* _name, const char* _dialogue)
{
	textColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("%s", _name);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(":%s\n\n", _dialogue);
}

void charDialogue(const char* _name, const char* _dialogue)
{
	textColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s", _name);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(" : %s\n\n", _dialogue);
}

void question(const char* _question)
{
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	printf("%s ", _question);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	gets_s(g_inputBuf, sizeof g_inputBuf);
}

void notice(const char* _notice)
{
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s", _notice);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void battleMessage(const char* _notice)
{
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s", _notice);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void message(const char* _notice)
{
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s", _notice);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
}

void helpDescription(const char* _command,const char* _description)
{
	textColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf("%s", _command);
	textColor(FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	printf(" - %s\n", _description);
}

void monsterStatusSet(int _index, const char* _name, const char* _say, int _hp, int _mp, int _atk, int _def, float _criPro, 
						int _dropGold, int _dropItemIndex, int _dropExp, float _itemDropPro)
{
	strcpy_s(g_monster[_index].name, _name);
	strcpy_s(g_monster[_index].say, _say);
	g_monster[_index].hp = _hp;
	g_monster[_index].mp = _mp;
	g_monster[_index].atk = _atk;
	g_monster[_index].def = _def;
	g_monster[_index].criPro = _criPro;
	g_monster[_index].dropExp = _dropExp;
	g_monster[_index].dropGold = _dropGold;
	g_monster[_index].dropItemIndex = _dropItemIndex;
	g_monster[_index].itemDropPro = _itemDropPro;
}

void monsterData()
{
	monsterStatusSet(0, "다람쥐", "앞니빨로 등껍질을 긁어주랴!", 50, 0, 5, 3, 5.0f, 10, 0, 10, 0.0f);
	monsterStatusSet(1, "날다람쥐", "나는 3초 날라다닌다.", 50, 0, 5, 5, 5.0f, 15, 0, 15, 0.0f);
	monsterStatusSet(2, "큰앞니 쥐", "뭐든 갉아먹는다!", 70, 0, 7, 5, 5.0f, 20, 0, 20, 0.0f);
}

void oneAreaSet(int _x, int _y, int _monsterIndex, int _eventIndex, int _npcIndex)
{
	g_map[_x][_y].monsterIndex = _monsterIndex;
	g_map[_x][_y].eventIndex = _eventIndex;
	g_map[_x][_y].npcIndex = _npcIndex;
}

void mapInfoSetting()
{
	oneAreaSet(200, 200, -1, -1, -1); // 현재 시작 위치 -1을 무효값으로 정했다.
	oneAreaSet(200, 199, 0, 0, 0);
	oneAreaSet(200, 201, 0, 0, 0);
	oneAreaSet(201, 200, 1, 0, 0);
	oneAreaSet(199, 200, 2, 0, 0);
}

void checkMapEvent()
{
	if (g_map[g_player.PosX][g_player.PosY].eventIndex != -1)
	{

	}

	if (g_map[g_player.PosX][g_player.PosY].monsterIndex != -1)
	{
		char tmp[256] = "";
		sprintf_s(tmp, 256, "%s이(가) 나타났습니다.\n\n", g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].name);
		notice(tmp);
	}

	if (g_map[g_player.PosX][g_player.PosY].npcIndex != -1)
	{

	}
}

void checkBattle()
{
	if (g_map[g_player.PosX][g_player.PosY].monsterIndex != -1)
	{
		int _monsterHP = g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].hp;

		while (1)
		{
			//몬스터가 주는 데미지
			int _monsterDamage = g_player.DEF - g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].atk;

			if (_monsterDamage >= 0)
				_monsterDamage = -1;

			bool _monsterCri = false;
			bool _playerCri = false;

			//크리티컬 처리
			int _prob = rand() % 100;
			if (g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].criPro > _prob)
			{
				_monsterDamage = _monsterDamage * 2;
				_monsterCri = true;
			}


			g_player.HP = g_player.HP + _monsterDamage;

			//플레이어가 주는 데미지
			int _playerDamage = g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].def - g_player.ATK;

			if (_playerDamage >= 0)
				_playerDamage = -1;

			//크리티컬 처리
			_prob = rand() % 100;
			if (g_player.CriProb > _prob)
			{
				_playerDamage = _playerDamage * 2;
				_playerCri = true;
			}

			g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].hp = g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].hp + _playerDamage;

			char tmp[256] = "";

			if (_playerCri == true)
				sprintf_s(tmp, 256, "%s은(는) %d의 크리티컬 데미지를 주었습니다.\n", g_player.name, -_playerDamage);
			else
				sprintf_s(tmp, 256, "%s은(는) %d데미지를 주었습니다.\n", g_player.name, -_playerDamage);

			battleMessage(tmp);
			Sleep(1000);

			if (_monsterCri == true)
				sprintf_s(tmp, 256, "%s은(는) %d의 크리티컬 데미지를 주었습니다.\n", g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].name, -_monsterDamage);
			else
				sprintf_s(tmp, 256, "%s은(는) %d데미지를 주었습니다.\n", g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].name, -_monsterDamage);

			battleMessage(tmp);
			Sleep(1000);
			if (g_player.HP <= 0)
			{
				char tmp[256] = "";
				sprintf_s(tmp, 256, "%s은(는) 죽었습니다.\n\n", g_player.name);
				battleMessage(tmp);
				break;
			}

			if (g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].hp <= 0)
			{
				char tmp[256] = "";
				sprintf_s(tmp, 256, "%s은(는) 죽었습니다.\n\n", g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].name);
				g_monster[g_map[g_player.PosX][g_player.PosY].monsterIndex].hp = _monsterHP;
				battleMessage(tmp);
				break;
			}

		}
	}
	else
	{
		battleMessage("대상이 없습니다.\n\n");
	}
}

void fantasyLandMap() {
	notice("기본적인 명령어와 사용법을 알고 싶으시면 '도움말' 명령어를 이용하세요. \n\n");
	while (1)
	{
		question("어디로 가시겠습니까? <동서남북> :");

		int _east = strcmp(g_inputBuf, "동");
		int _west = strcmp(g_inputBuf, "서");
		int _south = strcmp(g_inputBuf, "남");
		int _north = strcmp(g_inputBuf, "북");
		int _help = strcmp(g_inputBuf, "도움말");
		int _attack = strcmp(g_inputBuf, "공격");
		int _rest = strcmp(g_inputBuf, "휴식");
		int _position = strcmp(g_inputBuf, "위치");
		int _bag = strcmp(g_inputBuf, "가방");
		int _state = strcmp(g_inputBuf, "상태");
		int _equip = strcmp(g_inputBuf, "착용");
		int _buy = strcmp(g_inputBuf, "구입");
		int _sell = strcmp(g_inputBuf, "판매");
		int _look = strcmp(g_inputBuf, "보기");

		if (_east == 0)	g_player.playerCommandState = COMMAND_MOVE_EAST;
		if (_west == 0)	g_player.playerCommandState = COMMAND_MOVE_WEST;
		if (_south == 0)	g_player.playerCommandState = COMMAND_MOVE_SOUTH;
		if (_north == 0)	g_player.playerCommandState = COMMAND_MOVE_NORTH;
		if (_help == 0)	g_player.playerCommandState = COMMAND_HELP;
		if (_position == 0)	g_player.playerCommandState = COMMAND_POSITION;
		if (_bag == 0)	g_player.playerCommandState = COMMAND_BAG;
		if (_rest == 0)	g_player.playerCommandState = COMMAND_REST;
		if (_look == 0)	g_player.playerCommandState = COMMAND_LOOK;

		switch (g_player.playerCommandState)
		{
		case COMMAND_MOVE_EAST:
		{
			if (g_player.PosX < 499)
			{
				g_player.PosX++;
				checkMapEvent();
			}
			else
			{
				notice("막다른 길입니다. 다른곳으로 이동해주세요.\n\n");
			}
		}
		break;
		case COMMAND_MOVE_WEST:
		{
			if (g_player.PosX > 0)
			{
				g_player.PosX--;
				checkMapEvent();
			}
			else
			{
				notice("막다른 길입니다. 다른곳으로 이동해주세요.\n\n");
			}
		}
		break;
		case COMMAND_MOVE_SOUTH:
		{
			if (g_player.PosY < 499)
			{
				g_player.PosY++;
				checkMapEvent();
			}
			else
			{
				notice("막다른 길입니다. 다른곳으로 이동해주세요.\n\n");
			}
		}
		break;
		case COMMAND_MOVE_NORTH:
		{
			if (g_player.PosY > 0)
			{
				g_player.PosY--;
				checkMapEvent();
			}
			else
			{
				notice("막다른 길입니다. 다른곳으로 이동해주세요.\n\n");
			}
		}
		break;
		case COMMAND_HELP:
		{
			helpDescription("공격", "현재 위치의 누군가를 공격한다.");
			helpDescription("휴식", "현재 위치에서 휴식을 취한다. 휴식을 취하는 동안 HP,MP등이 회복된다.");
			helpDescription("위치", "현재 위치를 확인한다.");
			helpDescription("가방", "자신의 가방을 확인한다.");
			helpDescription("상태", "자신의 각종 능력치를 확인한다.");
			helpDescription("착용 아이템이름", "자신의 가방에 있는 아이템을 착용한다.");
			helpDescription("구입 아이템이름", "현재 위치의 상인에게 아이템을 구입한다.");
			helpDescription("판매 아이템이름", "현재 위치의 상인에게 아이템을 판매한다.");
			helpDescription("보기 이름", "자신의 아이템 또는 현재 위치의 누군가에 대한 정보를 확인한다.");
		}
		break;
		case COMMAND_ATTACK:
		{
			checkBattle();
		}
			break;
		case COMMAND_REST:
		{
			char tmp[256] = "";
			sprintf_s(tmp, 256, "%s은(는) 낮잠을 자기 시작합니다. 드르렁~ 쿨~~~\n\n", g_player.name);
			notice(tmp);
		}
		break;
		case COMMAND_POSITION:
		{
			char tmp[256] = "";
			sprintf_s(tmp, 256, "현재 위치는 (%d,%d)입니다.\n\n", g_player.PosX, g_player.PosY);
			notice(tmp);
		}
		break;
		case COMMAND_BAG:
		{
			notice("아직 가방이 없습니다.\n\n");
		}
		break;
		case COMMAND_STATE:
		{
			{
				char tmp[256] = "";
				sprintf_s(tmp, 256, "HP    %d/%d   MP %d/%d   ATK %d   DEF %d\nCRI  %3.2f       GOLD %d\n", g_player.HP, g_player.MAX_HP, g_player.MP, g_player.MAX_MP, g_player.ATK,
					g_player.DEF, g_player.CriProb, g_player.Gold);
				message(tmp);
			}
		}
			break;
		case COMMAND_EQUIP:
			break;
		case COMMAND_BUY:
			break;
		case COMMAND_SELL:
			break;
		case COMMAND_LOOK:
		{
			char tmp[256] = "";
			sprintf_s(tmp, 256, "이름:%s\n공격력:%d\n\n", g_monster[0].name, g_monster[0].atk);
			notice(tmp);
		}
		break;
		case COMMAND_WRONG:
			notice("꼬부기는 알 수 없는 말로 중얼거리고 있습니다.\n\n");
			break;
		}

		g_player.playerCommandState = COMMAND_WRONG;
	}

	//붉은털 게, 물개, 바다사자
}