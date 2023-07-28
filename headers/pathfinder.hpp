class Pathfinder{
	public:
		Pathfinder();
		void InitNodes();
		void DrawDefaultScreen();
		~Pathfinder();
		void RenderText(std::string TextString, SDL_Surface *TextSurface, int x, int y);
		void DrawDefaultSelectors();
		void CheckStartNode(int MapX, int MapY);
		void CheckEndNode(int MapX, int MapY);
		void CheckStartEndNodes(int MapX, int MapY);
		void MouseSelectionControls(int MapX, int MapY);
		void MouseDrawingControls(int MapX, int MapY);
		void MouseEraseControls(int MapX, int MapY);
		bool GetCommands();
		bool SolveAstar();
		void FinalPath();
		void Cycle();
	private:
		class Node{
			public:
				bool IsObstacle = false;
				bool IsVisited = false;
				float GlobalGoalDist;			// Distance to goal so far
				float LocalGoalDist;			// Distance to goal if we took the alternative route
				int x;
				int y;
				std::vector<Node*> Neighbours;
				Node* Parent;
		};
		enum class CellType{
			Empty = 0, Start, End, Wall, Explored, Edge
		};
		Node *Nodes = nullptr;
		Node *StartNode = nullptr;
		Node *EndNode = nullptr;
		SDL_Event Event;
		SDL_MouseButtonEvent MouseEvent;
		bool Quit;
		bool IsAlgorithmStarted;
		CellType Selector;
		SDL_Surface *Text;
};

Pathfinder::Pathfinder(){
	this->InitNodes();
	this->DrawDefaultScreen();
	Quit = false;
	IsAlgorithmStarted = false;
	Selector = CellType::Empty;
}

void Pathfinder::InitNodes(){

	Nodes = new Node[MapWidth * MapHeight];
	for(int x = 0; x < MapWidth; x++){
		for(int y = 0; y < MapHeight; y++){
			Nodes[y * MapWidth + x].x = x;
			Nodes[y * MapWidth + x].y = y;
			Nodes[y * MapWidth + x].IsObstacle = false;
			Nodes[y * MapWidth + x].Parent = nullptr;
			Nodes[y * MapWidth + x].IsVisited = false;
		}
	}

	for(int x = 0; x < MapWidth; x++){
		for(int y = 0; y < MapHeight; y++){
			if(y > 0)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y - 1) * MapWidth + (x + 0)]);
			if(y < MapHeight-1)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y + 1) * MapWidth + (x + 0)]);
			if (x > 0)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y + 0) * MapWidth + (x - 1)]);
			if(x < MapWidth-1)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y + 0) * MapWidth + (x + 1)]);
			if(y > 0 && x>0)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y - 1) * MapWidth + (x - 1)]);
			if(y < MapHeight-1 && x > 0)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y + 1) * MapWidth + (x - 1)]);
			if(y > 0 && x < MapWidth-1)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y - 1) * MapWidth + (x + 1)]);
			if(y < MapHeight - 1 && x < MapWidth-1)
				Nodes[y*MapWidth + x].Neighbours.push_back(&Nodes[(y + 1) * MapWidth + (x + 1)]);
		}
	}

}

void Pathfinder::DrawDefaultSelectors(){
	DrawSquare(MapWidth + MapWidth / 8, MapHeight / 5, Red, White);
	DrawSquare(MapWidth + MapWidth / 8, MapHeight / 5 + 2, Green, White);
	DrawSquare(MapWidth + MapWidth / 8, MapHeight / 5 + 4, Black, White);
}

Pathfinder::~Pathfinder(){
	Node *Nodes = nullptr;
	Node *StartNode = nullptr;
	Node *EndNode = nullptr;
}

void Pathfinder::RenderText(std::string TextString, SDL_Surface *TextSurface, int x, int y){
	TextSurface = TTF_RenderText_Shaded(Font, TextString.c_str(), Black, White);
	SDL_Rect TextRect = {x, y, 0, 0};
	SDL_BlitSurface(TextSurface, NULL, Surface, &TextRect);
	SDL_FreeSurface(TextSurface);
}

void Pathfinder::DrawDefaultScreen(){

	for(int i = 0; i < MapHeight * MapWidth; i++){
		DrawSquare(Nodes[i].x, Nodes[i].y, BackgroundColor);
	}

	for(int x = MapWidth * BlockSize; x < WindowWidth; x++){
		for(int y = 0; y < WindowHeight; y++){
			DrawPixel(x, y);
		}
	}

 	this->DrawDefaultSelectors();
	this->RenderText("-> start", Text, (MapWidth + MapWidth / 6) * BlockSize, (MapHeight / 5) * BlockSize);
	this->RenderText("-> end", Text, (MapWidth + MapWidth / 6) * BlockSize, (MapHeight / 5 + 2) * BlockSize);
	this->RenderText("-> wall", Text, (MapWidth + MapWidth / 6) * BlockSize, (MapHeight / 5 + 4) * BlockSize);
	this->RenderText("<<enter>> to find", Text, (MapWidth + MapWidth / MapWidth) * BlockSize, (MapHeight / 5 + 6) * BlockSize);

}

void Pathfinder::CheckStartNode(int MapX, int MapY){
	if(StartNode != nullptr){
		if(StartNode->x == MapX && StartNode->y == MapY)
			StartNode = nullptr;
	}
}

void Pathfinder::CheckEndNode(int MapX, int MapY){
	if(EndNode != nullptr){
		if(EndNode->x == MapX && EndNode->y == MapY)
			EndNode = nullptr;
	}
}

void Pathfinder::CheckStartEndNodes(int MapX, int MapY){
	this->CheckStartNode(MapX, MapY);
	this->CheckEndNode(MapX, MapY);
}

void Pathfinder::MouseSelectionControls(int MapX, int MapY){
	if(MapX == MapWidth + MapWidth / 8){
		if(MapY == MapHeight / 5){
			Selector = CellType::Start;
			this->DrawDefaultSelectors();
			DrawSquare(MapWidth + MapWidth / 8, MapHeight / 5, Red, Blue);
		}
		else if(MapY == MapHeight / 5 + 2){
			Selector = CellType::End;
			this->DrawDefaultSelectors();
			DrawSquare(MapWidth + MapWidth / 8, MapHeight / 5 + 2, Green, Blue);
		}
		else if(MapY == MapHeight / 5 + 4){
			Selector = CellType::Wall;
			this->DrawDefaultSelectors();
			DrawSquare(MapWidth + MapWidth / 8, MapHeight / 5 + 4, Black, Blue);
		}
	}
}

void Pathfinder::MouseDrawingControls(int MapX, int MapY){
	if(MapX < MapWidth){
		switch(Selector){
			case CellType::Start:
				if(StartNode == nullptr){
					Nodes[MapY * MapWidth + MapX].IsObstacle = false;
				  StartNode = &Nodes[MapY * MapWidth + MapX];
					DrawSquare(MapX, MapY, Red);
					this->CheckEndNode(MapX, MapY);
				}
				break;
			case CellType::End:
				if(EndNode == nullptr){
					Nodes[MapY * MapWidth + MapX].IsObstacle = false;
					EndNode = &Nodes[MapY * MapWidth + MapX];
					DrawSquare(MapX, MapY, Green);
					this->CheckStartNode(MapX, MapY);
				}
				break;
			case CellType::Wall:
				this->CheckStartEndNodes(MapX, MapY);
				Nodes[MapY * MapWidth + MapX].IsObstacle = true;
				DrawSquare(MapX, MapY, Black);
				break;
			default:
				break;
		}
	}
}

void Pathfinder::MouseEraseControls(int MapX, int MapY){
	if(MapX < MapWidth){
		this->CheckStartEndNodes(MapX, MapY);
		Nodes[MapY * MapWidth + MapX].IsObstacle = false;
		DrawSquare(MapX, MapY, BackgroundColor);
	}
}

bool Pathfinder::GetCommands(){

	while(SDL_WaitEvent(&Event)){

		if((Event.key.state == SDL_PRESSED && Event.key.keysym.sym == SDLK_ESCAPE) || Event.type == SDL_QUIT){
			Quit = true;
			break;
		}

		if(!IsAlgorithmStarted){
			if(Event.key.state == SDL_PRESSED){
				if(Event.key.keysym.sym == SDLK_RETURN && StartNode != nullptr && EndNode != nullptr){
					IsAlgorithmStarted = true;
					this->DrawDefaultSelectors();
		 			return true;
				}
			}
			if(Event.button.state == SDL_PRESSED || Event.button.state == SDL_RELEASED){
				int MapX, MapY;
				SDL_GetMouseState(&MapX, &MapY);
				MapX /= BlockSize;
				MapY /= BlockSize;
				if(Event.button.button == SDL_BUTTON_LEFT){
					this->MouseSelectionControls(MapX, MapY);
					this->MouseDrawingControls(MapX, MapY);
				}
				else if(Event.button.button == SDL_BUTTON_RIGHT || Event.button.button == SDL_BUTTON_RMASK){
					this->MouseEraseControls(MapX, MapY);
				}
			}
			SDL_UpdateWindowSurface(Window);
		}

	}

	return false;

}

bool Pathfinder::SolveAstar(){

	for (int x = 0; x < MapWidth; x++){
		for (int y = 0; y < MapHeight; y++){
			Nodes[y*MapWidth + x].IsVisited = false;
			Nodes[y*MapWidth + x].GlobalGoalDist = INFINITY;
			Nodes[y*MapWidth + x].LocalGoalDist = INFINITY;
			Nodes[y*MapWidth + x].Parent = nullptr;
		}
	}

	auto Distance = [](Node* a, Node* b){
		return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
	};

	auto Heuristic = [Distance](Node* a, Node* b){
		return Distance(a, b);
	};

	Node *CurrentNode = StartNode;
	StartNode->LocalGoalDist = 0.0f;
	StartNode->GlobalGoalDist = Heuristic(StartNode, EndNode);
	std::list<Node*> NotTestedNodes;
	NotTestedNodes.push_back(StartNode);

	while (!NotTestedNodes.empty() && CurrentNode != EndNode){

		NotTestedNodes.sort([](const Node* lhs, const Node* rhs){ return lhs->GlobalGoalDist < rhs->GlobalGoalDist; } );

		while(!NotTestedNodes.empty() && NotTestedNodes.front()->IsVisited)
			NotTestedNodes.pop_front();

		if(NotTestedNodes.empty())
			break;

		CurrentNode = NotTestedNodes.front();
		CurrentNode->IsVisited = true;

		for(auto NeighbourNode : CurrentNode->Neighbours){
			if (!NeighbourNode->IsVisited && !NeighbourNode->IsObstacle)
				NotTestedNodes.push_back(NeighbourNode);
			float PossiblyLowerGoal = CurrentNode->LocalGoalDist + Distance(CurrentNode, NeighbourNode);
			if(PossiblyLowerGoal < NeighbourNode->LocalGoalDist){
				NeighbourNode->Parent = CurrentNode;
				NeighbourNode->LocalGoalDist = PossiblyLowerGoal;
				NeighbourNode->GlobalGoalDist = NeighbourNode->LocalGoalDist + Heuristic(NeighbourNode, EndNode);
			}
		}

		DrawSquare(CurrentNode->x, CurrentNode->y, ExpansionColor);
		DrawSquare(EndNode->x, EndNode->y, Green);
		DrawSquare(StartNode->x, StartNode->y, Red);

		SDL_UpdateWindowSurface(Window);
		SDL_Delay(20);

	}

	return true;

}

void Pathfinder::FinalPath(){
	if(EndNode->Parent == nullptr)
		return;
 	Node *End = EndNode->Parent;
	while(End->Parent != nullptr){
		DrawSquare(End->x, End->y, PathColor);
		DrawSquare(EndNode->x, EndNode->y, Green);
		SDL_UpdateWindowSurface(Window);
		SDL_Delay(50);
		End = End->Parent;
	}
}

void Pathfinder::Cycle(){
	while(!Quit){
		if(this->GetCommands()){
			this->SolveAstar();
			this->FinalPath();
		}
	}
}
