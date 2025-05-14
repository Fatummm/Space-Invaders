# Space Invaders
There is a **Space Invaders** realization on C++

---

## Navigation
- [Game description](#game-descrption)
- [How to install and run](#how-to-install-and-run)
- [Game realization](#game-realization)
- [Technologies stack](#technologies-stack)
- [Examples]()

## Game Description
__Space Invaders__ is the biggest hit in the history of arcade games and the earliest example of a single-player shooting game. 

### Description
Each level tasks the player with destroying an 8 x 5 grid of aliens. The wave initially moves right, shifting down a row and reversing direction upon reaching either side of the screen. The speed depends on the chosen difficulty. The cannon can only move horizontally, and shoot straight up. The cannon can shoot once a second. Each level has four destructible shields that block both player and enemy shots. The player starts with three lives. If all lives are lost, the game ends.

![asd](examples/gameplay.png)

### Scoring

#### For enemies:
- __Green__![green](src/images/enemies/tier1-1.png)(octopus) - 10 points
- __Violet__![green](src/images/enemies/tier2-1.png)(crab) - 20 points
- __Red__![green](src/images/enemies/tier3-1.png)(Squid) - 30 points


#### After defeating the whole army
- __Extra__ points are added for walls(70 for each one)
- __Total score__ is multiplied in the number of left lives of the player.

__Maximum score__ is 3000 points.

## How to Install and Run
:bangbang:This projects works __ONLY on Linux__:bangbang:

1. Firstly, install SFML for C++:
    ```
    sudo apt-get install libsfml-dev
    ```
    If this command __doesn't work__, visit [official SFML site](https://www.sfml-dev.org/tutorials/3.0/getting-started/linux/#installing-sfml).

2. Then install CMake:
    ```
    sudo apt install cmake
    ```
3. Clone this repository and open it
4. Open `/build` and run:
    ```
    cmake ..
    make
    ```
5. To Run the app, leave build and run the Game file:
    ```
    ./Game
    ```

## Game Realization
The game was created with using OOP - Object-Oriented Programming with the following class hierarchy:
```
├── SFMLObject
│   ├── Entity
│   │   ├── Player
│   │   ├── Enemy
│   │   └── Bullet
│   ├── Wall
│   └── Button
├── Page
│   ├── MainMenuPage
│   ├── GamePage
│   ├── ControlsPage
│   ├── DifficultyPage
│   ├── VictoryPage
│   └── LosePage
├── Score
├── Army
└── Game
```
`Score` and `Army` classes use the idea of composition: they have objects of type `SFMLObject` and `Enemy` as fields, that's why they are not in `SFMLObject`-tree.

## Technologies Stack
- CMake for creating project
- SFML for GUI
- pure C++
- Aseprite for creating sprites' images
