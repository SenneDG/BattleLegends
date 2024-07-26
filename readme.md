 # Final Project
 ## UML
![UML](images/images/UML_07012024.png)

## PROGRESS
### Nov 15
- Created the initial UML that contained the basic structure - All
- Setting up de world library - All
- Set up the structure in Qt - All
- Created a first implementation of the controller, view and gamemodel - All

### Nov 22
- Created moveTo function in gameModel that allows the protagonist to move to a certain position - Jacob
- Getting the tiles, enemies and healthpacks from the world library to gameModel - Senne
- Catch keyPressEvent and print them in terminal - Jacob
- Created first version of the graphicView (displaying the worldmap) - Maarten

### Nov 29
- Second version of the UML is uploaded - Senne
- Got moveTo function working in textView using keypresses - Jacob
- First implementation of consuming healthpacks - Jacob
- First implementation of the attack function in the gameModel (Enemy and PEnemy) - Senne
- First implementation of handling poisoneffect - Senne
- Energy changing when protagonist moves around the world - Jacob
- Enemies can only be defeated once - Jacob

### Dec 6 
- Made the textView initialisation work - Jacob
- Protagonist moves around the textView - Jacob
- TextView changed so it only loads a part of the world and the world moves with protagonist - Jacob
- Refactoring of the attack functionality, now making use of the poison() signals - Senne

### Dec 13
- Added enemyTextView to represent enemies on the textView - Jacob
- First implementation of the MyXEnemy type (ranged attacker) - Senne
- Creating 5% MyXEnemy types in the enemies list - Senne
- Changed tiles, healthpacks and enemies to shared pointers - Jacob
- Added an enemy death visualization on the textView - Jacob

### Dec 20
- Adding PEnemyTextView, MyXEnemyTextView to visulize enemies on textView - Jacob
- Changing color when the protagonist takes damage - Jacob
- Protagonist is now visible on GraphicView - Maarten
- Protagonist can move with keys on the GraphicView - Maarten
- Added command for going up, down, right and left - Senne
- Added healthbar and energybar that shows the health and energy of the protagonist - Jacob

### Dec 23
- Created MyHealthpack class. Now healthpacks can only be consumed once - Jacob
- Visualisation of the enemies and healthpacks on the graphicView - Maarten
- Added an overlay on the graphicView (grass, sand and walls) - Maarten
- Updated the PEnemy poison implementation. Now it will create poisonTiles around the PEnemy - Senne
- Updated the handling of the poisonlevel in the textView with increasing range - Senne

### Dec 25
- Added seperate classes for visualization of enemies and healthpack in the GraphicView - Maarten
- First implementation of the pathFindingAlgorithm working with goto command - Senne
- DrawPath function in textView to show the path - Senne
- Healthpacks now get removed from the world when been consumed - Jacob
- GameOver window when the protagonist dies - Jacob
- Toggling between textView and graphicView - Jacob

### Dec 27
- Created a third version of the UML - Senne
- Created resource.qrc file to store images effectifly - Senne
- Added heuristicWeight slider to change the distWeight in the pathfindingAlgorithm - Senne
- Added animationDelay slider to change the delay of the pathfinding animation - Senne
- Added commands to find nearest enemy and healthpack - Senne
- Added levelmanager to manage the different levels - Jacob
- Implemented Strategy class to create the autoplay function. Now the autoplay can kill all enemies in 1 level - Senne
- Added startscreen of the game - Senne
- Update on the GraphicScene. Now everything goes smoothly in the GraphicView - Maarten
- Started working on saving the gamestate - Senne
- Started working on loading a saved gamestate - Senne


### Dec 29
- Poison visualization in the GraphicView - Maarten
- Made the switching levels work also in the graphicView - Maarten
- Representation of doorTiles in graphicView - Maarten
- Made all neccesary functions in mainwindow work with signal and slots - Senne
- Refactoring the code - All
- autoplay, commands and mousePressEvent now working on GraphicView - Senne

### Dec 31
- updated the graphicView so that is works with multiple maps - Maarten 
- Made sure that the amount of enmies and healthpacks are mapsize dependent - Maarten 
- Inlining of simple getters and setters - Senne
- Changed the attack methods using virtual method to avoid dynamic_cast - Senne
- Refactored GameModel, Graphicview and Strategy to pass-by-reference - Senne
- Connected signals and slots in Graphicview - Senne
- Implemented portals to next level - Maarten 

### Jan 7
- Created fourth version of the UML - Senne
- Did general refactoring memory management and clean code to make the game run smoother - All
- Added third view Pathfindingview that renders the full image to show pathfinding - Senne
- Added a StartWindow that allows the user to upload own image - Senne
- Changed some heavy loops to make use of STL algorithms and functors - Senne
- Made the MouseClickEvent on the GraphicView and PathfindinView - Senne
- Visualization of the doorTile in the textview - Jacob
- Created a GameOver view - Jacob
- Added the help instructions in game - Jacob







