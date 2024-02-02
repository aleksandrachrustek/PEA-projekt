#pragma once
class Menu
{
	int numberOfVertices;
	int **vertices;
	std::string filename;
	int LoadFile(std::string &filename);

public:
	Menu();
	~Menu();
	void ShowMenu();
};


