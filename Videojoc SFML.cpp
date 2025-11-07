#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <ctime>
#include <iostream>
#include <iomanip>
#include "funcionsaux.h"

using namespace sf;
using namespace std;

int main()
{
	srand((int)time(0));

	// ============================================================================
	// BLOQUE 1: CONFIGURACIÓN DE LA VENTANA DEL JUEGO
	// ============================================================================
	// Se crea una ventana de 1920x1080 en modo pantalla completa
	// Esta será la ventana principal donde se renderiza todo el juego "Timber!!!"
	VideoMode vm({ 1920, 1080 });
	RenderWindow window(vm, "Timber!!!", State::Fullscreen);

	// ============================================================================
	// BLOQUE 2: CARGA Y CONFIGURACIÓN DEL FONDO
	// ============================================================================
	// Se carga la textura del fondo del juego y se crea el sprite correspondiente
	// Este será el fondo estático que se dibuja primero en cada frame
	Texture textureBackground("graphics/background.png");
	Sprite spriteBackground(textureBackground);

	// ============================================================================
	// BLOQUE 3: ÁRBOLES DECORATIVOS DE FONDO
	// ============================================================================
	// Se crean dos árboles adicionales para dar profundidad visual al escenario
	// Estos árboles son meramente decorativos y se colocan detrás del árbol principal
	Texture textureTreeBack("graphics/tree2.png");
	Sprite treeBack1(textureTreeBack);
	Sprite treeBack2(textureTreeBack);

	// Posicionamiento de los árboles decorativos: uno a la izquierda y otro a la derecha
	treeBack1.setPosition({ 400, 0 });
	treeBack2.setPosition({ 1400, 0 });

	// ============================================================================
	// BLOQUE 4: ÁRBOL PRINCIPAL (ELEMENTO CENTRAL DEL JUEGO)
	// ============================================================================
	// Este es el árbol que el jugador está talando y donde aparecen las ramas
	// Se posiciona en el centro de la pantalla
	Texture textureTree1("graphics/tree.png");
	Sprite spriteTree1(textureTree1);
	spriteTree1.setPosition({ 810, 0 });

	// ============================================================================
	// BLOQUE 5: CONFIGURACIÓN DEL JUGADOR
	// ============================================================================
	// Se carga la textura del personaje jugador y se posiciona inicialmente
	// en el lado izquierdo del árbol
	Texture texturePlayer("graphics/player.png");
	Sprite spritePlayer(texturePlayer);
	spritePlayer.setPosition({ 580, 720 });

	// ============================================================================
	// BLOQUE 6: ENTIDADES MÓVILES (ABEJA Y NUBES)
	// ============================================================================
	// Se crean elementos decorativos animados que se mueven por la pantalla
	// La abeja y las nubes usan la clase NPC para gestionar su movimiento
	Texture textureBee("graphics/bee.png");
	Texture textureCloud("graphics/cloud.png");

	// La abeja se mueve de derecha a izquierda con velocidad -1
	NPC bee(textureBee, 500, 400, -1, 2000);

	// Array de 3 nubes que se mueven de izquierda a derecha con velocidad 1
	NPC clouds[NUM_CLOUDS] = {
		NPC(textureCloud, 100, 200, 1, -200),
		NPC(textureCloud, 250, 200, 1, -200),
		NPC(textureCloud, 500, 200, 1, -200)
	};

	// ============================================================================
	// BLOQUE 7: SISTEMA DE RAMAS DEL ÁRBOL
	// ============================================================================
	// Se configuran las 6 ramas que pueden aparecer en el árbol
	// Cada rama puede estar a la izquierda, derecha o ausente (NONE)
	Texture textureBranch("graphics/branch.png");
	Side branchPositions[NUM_BRANCHES]; // Array que guarda la posición de cada rama

	// Array de sprites para las 6 ramas del árbol
	Sprite branches[NUM_BRANCHES] = {
		Sprite(textureBranch),Sprite(textureBranch),Sprite(textureBranch),
		Sprite(textureBranch),Sprite(textureBranch),Sprite(textureBranch)
	};

	// Inicialización de cada rama: se colocan fuera de pantalla y sin posición asignada
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition({ -2000, -2000 }); // Fuera de la pantalla
		branches[i].setOrigin({ 220, 20 }); // Punto de origen para rotación
		branchPositions[i] = Side::NONE; // Sin rama inicialmente
	}

	// ============================================================================
	// BLOQUE 8: CONFIGURACIÓN DEL HACHA
	// ============================================================================
	// El hacha cambia de posición según el lado donde está el jugador
	// Se definen las constantes para las posiciones izquierda y derecha
	Texture textureAxe("graphics/axe.png");
	Sprite spriteAxe(textureAxe);
	spriteAxe.setPosition({ 660, 830 });
	const float AXE_POSITION_LEFT = 660;
	const float AXE_POSITION_RIGHT = 1010;

	// ============================================================================
	// BLOQUE 9: CONFIGURACIÓN DEL TRONCO VOLADOR
	// ============================================================================
	// Cuando el jugador tala, aparece un tronco que sale volando
	// El tronco se mueve horizontalmente y hacia arriba con física simple
	Texture textureLog("graphics/log.png");
	Sprite spriteLog(textureLog);
	spriteLog.setPosition({ -2000, 660 }); // Fuera de pantalla inicialmente
	bool logActive = false; // Indica si el tronco está en movimiento
	float logSpeedX = 1000; // Velocidad horizontal del tronco
	float logSpeedY = -1500; // Velocidad vertical del tronco (negativa = hacia arriba)

	// ============================================================================
	// BLOQUE 10: CONFIGURACIÓN DE LA LÁPIDA (GAME OVER)
	// ============================================================================
	// Cuando el jugador pierde, aparece una lápida (RIP)
	// Inicialmente está fuera de la pantalla
	Texture textureRIP("graphics/rip.png");
	Sprite spriteRIP(textureRIP);
	spriteRIP.setPosition({ 675, 2000 }); // Fuera de pantalla hasta que el jugador muera

	// ============================================================================
	// BLOQUE 11: CONFIGURACIÓN DE TEXTOS Y PUNTUACIÓN
	// ============================================================================
	// Se carga la fuente y se configuran los textos para mostrar información
	Font font("fonts/KOMIKAP_.ttf");

	// Texto del mensaje principal (inicio, game over, etc.)
	Text messageText(font);
	messageText.setString("Press Enter to start!");
	messageText.setCharacterSize(75);
	messageText.setFillColor(Color::White);
	// Centrado del texto en la pantalla
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin({ textRect.size.x / 2.0f, textRect.size.y / 2.0f });
	messageText.setPosition({ 1920.0f / 2.0f, 1080.0f / 2.0f });

	// Texto de la puntuación que aparece en la esquina superior izquierda
	Text scoreText(font);
	scoreText.setCharacterSize(100);
	scoreText.setString("Score = 0");
	scoreText.setFillColor(Color::White);
	scoreText.setPosition({ 20, 20 });

	// ============================================================================
	// BLOQUE 12: FONDO SEMITRANSPARENTE PARA LA PUNTUACIÓN (HUD)
	// ============================================================================
	// Rectángulo negro semitransparente detrás del texto de puntuación
	// Mejora la legibilidad del texto sobre el fondo del juego
	RectangleShape scoreBackground({ 350, 120 });
	scoreBackground.setFillColor(Color(0, 0, 0, 150)); // Negro con transparencia
	scoreBackground.setPosition({ 10, 10 });

	// ============================================================================
	// BLOQUE 13: CONSOLA DE DEPURACIÓN (FPS Y ESTADÍSTICAS)
	// ============================================================================
	// Textos para mostrar información de depuración en tiempo real

	// Texto que muestra los FPS (frames por segundo)
	Text fpsText(font);
	fpsText.setCharacterSize(30);
	fpsText.setFillColor(Color::Yellow);
	fpsText.setPosition({ 20, 150 });

	// Texto que muestra estadísticas del juego (vidas, dificultad, etc.)
	Text debugText(font);
	debugText.setCharacterSize(25);
	debugText.setFillColor(Color::Cyan);
	debugText.setPosition({ 20, 200 });

	// Fondo semitransparente para la consola de depuración
	RectangleShape debugBackground({ 400, 200 });
	debugBackground.setFillColor(Color(0, 0, 0, 180));
	debugBackground.setPosition({ 10, 140 });

	// ============================================================================
	// BLOQUE 14: VARIABLES PARA EL CÁLCULO DE FPS
	// ============================================================================
	// Sistema para calcular y actualizar los FPS de forma eficiente
	Clock fpsClock; // Reloj para medir el tiempo entre frames
	int frameCount = 0; // Contador de frames renderizados
	float fpsUpdateTime = 0.0f; // Tiempo acumulado para actualización de FPS
	float currentFPS = 0.0f; // FPS actual calculado
	const float FPS_UPDATE_INTERVAL = 0.25f; // Intervalo de actualización (4 veces por segundo)

	// ============================================================================
	// BLOQUE 15: VARIABLES PRINCIPALES DEL ESTADO DEL JUEGO
	// ============================================================================
	// Variables que controlan el estado y lógica del juego
	bool paused = true; // El juego empieza pausado
	Clock clock; // Reloj principal para calcular el delta time
	int score = 0; // Puntuación del jugador
	float timeRemaining = 6.0f; // Tiempo restante antes de perder
	Side playerSide = Side::LEFT; // Lado donde está el jugador (izquierda o derecha)
	float difficultyMultiplier = 1.0f; // Multiplicador de dificultad que aumenta con el score
	int lives = 3; // Vidas del jugador

	// ============================================================================
	// BLOQUE 16: BARRA DE TIEMPO (TIME BAR)
	// ============================================================================
	// Barra visual que muestra el tiempo restante
	// Se reduce progresivamente y cuando llega a 0 el jugador pierde
	RectangleShape timeBar({ timeBarStartWidth, timeBarHeight });
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition({ 1920.0f / 2.0f - timeBarStartWidth / 2.0f, 980 }); // Centrada en la parte inferior
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining; // Píxeles por segundo

	// ============================================================================
	// BLOQUE 17: BUCLE PRINCIPAL DEL JUEGO
	// ============================================================================
	// Este bucle se ejecuta continuamente mientras la ventana esté abierta
	// Contiene toda la lógica de eventos, actualización y renderizado
	while (window.isOpen()) {

		// ========================================================================
		// BLOQUE 18: GESTIÓN DE EVENTOS (INPUT DEL USUARIO)
		// ========================================================================
		// Procesa todos los eventos de la ventana: teclas, cierre, etc.
		while (const optional event = window.pollEvent()) {

			// Si el usuario cierra la ventana, se termina el juego
			if (event->is<Event::Closed>())
				window.close();

			// Procesamiento de eventos de teclado
			if (const auto* keyEvent = event->getIf<Event::KeyPressed>()) {

				// Tecla ESC: cierra el juego
				if (keyEvent->code == Keyboard::Key::Escape) {
					window.close();
				}

				// ============================================================
				// BLOQUE 19: TECLA ENTER - INICIAR/REINICIAR PARTIDA
				// ============================================================
				// Cuando se presiona Enter, se reinicia el juego con valores por defecto
				if (keyEvent->code == Keyboard::Key::Enter) {
					paused = false; // Despausa el juego
					score = 0; // Reinicia la puntuación
					timeRemaining = 6; // Reinicia el tiempo
					lives = 3; // Reinicia las vidas
					difficultyMultiplier = 1.0f; // Reinicia la dificultad

					// Limpia todas las ramas del árbol
					for (int i = 0; i < NUM_BRANCHES; i++)
						branchPositions[i] = Side::NONE;

					// Reinicia posiciones de sprites
					spriteRIP.setPosition({ 675, 2000 }); // Oculta la lápida
					spritePlayer.setPosition({ 580, 720 }); // Jugador a la izquierda
					spriteAxe.setPosition({ AXE_POSITION_LEFT, 830 }); // Hacha a la izquierda
					playerSide = Side::LEFT; // Jugador empieza en el lado izquierdo
				}

				// ============================================================
				// BLOQUE 20: TECLA DERECHA - TALAR DESDE LA DERECHA
				// ============================================================
				// El jugador se mueve al lado derecho y tala el árbol
				if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
					playerSide = Side::RIGHT; // Cambia al lado derecho
					score++; // Incrementa la puntuación

					// Aumenta la dificultad progresivamente con la puntuación
					difficultyMultiplier = 1.0f + (score / 50.0f);
					// Añade tiempo (menos tiempo cuanto mayor es el score)
					timeRemaining += (2.0f / score) + 0.15f;

					// Actualiza posiciones del hacha y jugador al lado derecho
					spriteAxe.setPosition({ AXE_POSITION_RIGHT, 830 });
					spritePlayer.setPosition({ 1120, 720 });

					// Actualiza las ramas (las hace bajar)
					updateBranches(branchPositions, score);

					// Activa el tronco volador hacia la derecha
					spriteLog.setPosition({ 810, 720 });
					logSpeedX = 5000; // Velocidad positiva = hacia la derecha
					logActive = true;
				}

				// ============================================================
				// BLOQUE 21: TECLA IZQUIERDA - TALAR DESDE LA IZQUIERDA
				// ============================================================
				// El jugador se mueve al lado izquierdo y tala el árbol
				if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
					playerSide = Side::LEFT; // Cambia al lado izquierdo
					score++; // Incrementa la puntuación

					// Aumenta la dificultad progresivamente con la puntuación
					difficultyMultiplier = 1.0f + (score / 50.0f);
					// Añade tiempo (menos tiempo cuanto mayor es el score)
					timeRemaining += (2.0f / score) + 0.15f;

					// Actualiza posiciones del hacha y jugador al lado izquierdo
					spriteAxe.setPosition({ AXE_POSITION_LEFT, 830 });
					spritePlayer.setPosition({ 580, 720 });

					// Actualiza las ramas (las hace bajar)
					updateBranches(branchPositions, score);

					// Activa el tronco volador hacia la izquierda
					spriteLog.setPosition({ 810, 720 });
					logSpeedX = -5000; // Velocidad negativa = hacia la izquierda
					logActive = true;
				}
			}
		}

		// ========================================================================
		// BLOQUE 22: ACTUALIZACIÓN DEL JUEGO (LÓGICA Y FÍSICA)
		// ========================================================================
		// Solo se actualiza el juego si NO está pausado
		if (!paused) {
			// Obtiene el tiempo transcurrido desde el último frame (delta time)
			Time dt = clock.restart();

			// ============================================================
			// BLOQUE 23: ACTUALIZACIÓN DEL TIEMPO RESTANTE Y TIME BAR
			// ============================================================
			// Reduce el tiempo restante y actualiza el tamaño de la barra
			timeRemaining -= dt.asSeconds();
			timeBar.setSize({ timeBarWidthPerSecond * timeRemaining, timeBarHeight });

			// Si el tiempo se agota, el jugador pierde
			if (timeRemaining <= 0) {
				paused = true;
				messageText.setString("Out of time!");
				// Recentra el mensaje
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin({ textRect.size.x / 2.0f, textRect.size.y / 2.0f });
			}

			// ============================================================
			// BLOQUE 24: ACTUALIZACIÓN DE ENTIDADES MÓVILES
			// ============================================================
			// Actualiza la posición de las nubes y la abeja
			updateNPC(clouds[0], dt.asSeconds());
			updateNPC(clouds[1], dt.asSeconds());
			updateNPC(clouds[2], dt.asSeconds());
			updateNPC(bee, dt.asSeconds());

			// ============================================================
			// BLOQUE 25: ACTUALIZACIÓN VISUAL DE LAS RAMAS
			// ============================================================
			// Actualiza las posiciones gráficas de las ramas según branchPositions
			updateBranchSprites(branchPositions, branches);

			// ============================================================
			// BLOQUE 26: DETECCIÓN DE COLISIÓN CON RAMA
			// ============================================================
			// Comprueba si hay una rama en la posición del jugador (posición 5)
			// Si hay colisión, el jugador muere
			if (branchPositions[5] == playerSide) {
				paused = true; // Pausa el juego
				lives--; // Reduce las vidas
				spriteRIP.setPosition({ 525, 760 }); // Muestra la lápida
				spritePlayer.setPosition({ 2000, 660 }); // Oculta al jugador
				messageText.setString("SQUISHED!"); // Mensaje de muerte
				// Recentra el mensaje
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin({ textRect.size.x / 2.0f, textRect.size.y / 2.0f });
			}

			// ============================================================
			// BLOQUE 27: ACTUALIZACIÓN DEL TEXTO DE PUNTUACIÓN
			// ============================================================
			// Actualiza el texto que muestra la puntuación actual
			stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// ============================================================
			// BLOQUE 28: FÍSICA DEL TRONCO VOLADOR
			// ============================================================
			// Si el tronco está activo, actualiza su posición
			if (logActive) {
				// Mueve el tronco según sus velocidades X e Y
				spriteLog.setPosition({
					spriteLog.getPosition().x + logSpeedX * dt.asSeconds(),
					spriteLog.getPosition().y + logSpeedY * dt.asSeconds()
					});

				// Si el tronco sale de la pantalla, lo desactiva
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
					logActive = false;
					spriteLog.setPosition({ 810, 720 }); // Lo devuelve a su posición inicial
				}
			}
		}
		else {
			// Si el juego está pausado, reinicia el reloj para evitar saltos de tiempo
			clock.restart();
		}

		// ========================================================================
		// BLOQUE 29: ACTUALIZACIÓN DEL CONTADOR DE FPS
		// ========================================================================
		// Sistema que calcula los FPS promedio cada cierto intervalo
		frameCount++; // Incrementa el contador de frames
		fpsUpdateTime += fpsClock.restart().asSeconds(); // Acumula el tiempo

		// Cada FPS_UPDATE_INTERVAL segundos, calcula y actualiza los FPS
		if (fpsUpdateTime >= FPS_UPDATE_INTERVAL) {
			currentFPS = frameCount / fpsUpdateTime; // Calcula FPS promedio
			frameCount = 0; // Reinicia el contador
			fpsUpdateTime = 0.0f; // Reinicia el tiempo acumulado

			// ============================================================
			// BLOQUE 30: ACTUALIZACIÓN DE TEXTOS DE DEPURACIÓN
			// ============================================================
			// Actualiza el texto de FPS
			stringstream fpsStream;
			fpsStream << fixed << setprecision(1) << "FPS: " << currentFPS;
			fpsText.setString(fpsStream.str());

			// Actualiza el texto con estadísticas del juego
			stringstream debugStream;
			debugStream << fixed << setprecision(2);
			debugStream << "Vides: " << lives << "\n";
			debugStream << "Dificultat: x" << difficultyMultiplier << "\n";
			debugStream << "Temps restant: " << timeRemaining << "s\n";
			debugStream << "Estat: " << (paused ? "PAUSAT" : "ACTIU");
			debugText.setString(debugStream.str());
		}

		// ========================================================================
		// BLOQUE 31: RENDERIZADO (DIBUJADO EN PANTALLA)
		// ========================================================================
		// Se dibuja todo en orden: primero el fondo, luego elementos intermedios
		// y finalmente elementos del frente. El orden es crucial para las capas.

		window.clear(); // Limpia la pantalla para el nuevo frame

		// Dibuja el fondo del juego
		window.draw(spriteBackground);

		// Dibuja los árboles decorativos (están detrás de todo)
		window.draw(treeBack1);
		window.draw(treeBack2);

		// Dibuja las nubes (capa intermedia)
		for (int i = 0; i < NUM_CLOUDS; i++) {
			window.draw(clouds[i].sprite);
		}

		// Dibuja las ramas del árbol
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// Dibuja el árbol principal (delante de los decorativos)
		window.draw(spriteTree1);

		// Dibuja al jugador y la abeja
		window.draw(spritePlayer);
		window.draw(bee.sprite);

		// Dibuja la interfaz de usuario (HUD)
		window.draw(scoreBackground); // Fondo de la puntuación
		window.draw(scoreText); // Texto de la puntuación

		// Dibuja la consola de depuración
		window.draw(debugBackground);
		window.draw(fpsText);
		window.draw(debugText);

		// Dibuja elementos del juego
		window.draw(timeBar); // Barra de tiempo
		window.draw(spriteAxe); // Hacha
		window.draw(spriteLog); // Tronco volador
		window.draw(spriteRIP); // Lápida (solo visible si el jugador muere)

		// Si el juego está pausado, dibuja el mensaje central
		if (paused) window.draw(messageText);

		// Muestra todo lo dibujado en la pantalla
		window.display();
	}

	return 0; // Termina el programa correctamente
}