# Odbicia-powierzchni-plaskich-OpenGL
Projekt z algorytmów grafiki komputerowej (odbicia powierzchni płaskich)

## Algorytm:

Algorytm którego użyłem do stworzenia odbić polega na prostej zmianie pozycji
kamery na osi y i rotacji względem który ma odbijać scenę. Przykładowo, kiedy kamera jest na
wysokości 20 nad lustrem którego y = 0, to algorytm zmienia tą pozycję na wysokość -20. Rotacja
kamery również jest odbijana względem obiektu lustra. Ilustracja takiego przekształcenia pokazana
została na rysunku 1.1. Następnie przekształcona kamera renderuje scenę do tzw. „frame buffor’a”
(buffora ramki). Po zakończeniu procesu renderowania tekstura zostaje nałożona na lustro.
Następnie czyszczone są buffory i renderowana jest właściwa scena z oryginalnej pozycji kamery
która zostaje wyświetlona na ekranie.

![alt text](https://github.com/Yorshka-Vermilion/Odbicia-powierzchni-plaskich-OpenGL/blob/Help/1.PNG?raw=true)

## Biblioteki:

- stb
- OBJLoader
- glew
- glfw
- glm

## Instrukcja obsługi:

Program możemy uruchomić na dwa sposoby. Pierwszym z nich jest wejście do folderu
x64/Release i uruchomienie pliku OpenGL-Odbicia.exe. Drugim sposobem jest uruchomienie
pliku OpenGL-Odbicia.sln w programie VisualStudio, a następnie skompilowaniu projektu. Do
uruchomienia programu w edytorze IDE, trzeba będzie zlinkować biblioteki

### Sterowanie: 

W,S,A,D – ruch kamery

LShift – podniesienie kamery

LCtrl – opuszczenie kamery

LPM i Ruch myszką – rotacja kamery

### Konfiguracja:

Aby dodać więcej obiektów do sceny, należy przejść do pliku OpenGL-Odbicia.cpp, a
następnie zjechać do linijki 240 w której napisany został komentarz „//::Obiekty::”. Pod tym
komentarzem możemy dodawać obiekty do sceny za pomocom funkcji addRenderObject().
Przyjmuje ona trzy argumenty, nowy obiekt typu RenderObject, drugi to ścieżka do tekstury, a
trzeci jest to zmienna typu bool, określająca czy obiekt ma odbijać obraz (domyślnie ). W
konstruktorze obiektu RenderObject należy podać następujące argumenty: ścieżkę do pliku
obiektu w formacie .obj, przesunięcie (glm::vec3), rotację (glm::vec3), oraz skalę (glm::vec3).

## Screeny:

![alt text](https://github.com/Yorshka-Vermilion/Odbicia-powierzchni-plaskich-OpenGL/blob/Help/2.PNG?raw=true)
![alt text](https://github.com/Yorshka-Vermilion/Odbicia-powierzchni-plaskich-OpenGL/blob/Help/3.PNG?raw=true)
