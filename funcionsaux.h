#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

// ============================================================================
// BLOQUE 1: CONSTANTES GLOBALES DEL JUEGO
// ============================================================================
// Estas constantes definen valores fijos que se usan en múltiples partes del juego
// NUM_CLOUDS: Número total de nubes que aparecen en el cielo
const int NUM_CLOUDS = 3;

// timeBarStartWidth: Ancho inicial de la barra de tiempo en píxeles
// Esta barra se reduce conforme pasa el tiempo
const float timeBarStartWidth = 400;

// timeBarHeight: Altura de la barra de tiempo en píxeles
const float timeBarHeight = 80;

// NUM_BRANCHES: Número de ramas visibles en el árbol
// El juego muestra 6 niveles de ramas simultáneamente
const int NUM_BRANCHES = 6;

// ============================================================================
// BLOQUE 2: ENUMERACIÓN PARA POSICIONES
// ============================================================================
// Define los tres estados posibles para las posiciones de ramas y jugador:
// - LEFT: Elemento está en el lado izquierdo del árbol
// - RIGHT: Elemento está en el lado derecho del árbol
// - NONE: No hay elemento (usado para ramas vacías)
enum class Side { LEFT, RIGHT, NONE };

// ============================================================================
// BLOQUE 3: ESTRUCTURA NPC (NON-PLAYABLE CHARACTER)
// ============================================================================
// Esta estructura define entidades móviles decorativas como abejas y nubes
// Cada NPC tiene su propio sprite, estado de actividad y parámetros de movimiento
struct NPC {
	// ========================================================================
	// ATRIBUTOS VISUALES Y DE ESTADO
	// ========================================================================
	Sprite sprite;        // Sprite visual del NPC (imagen que se dibuja)
	bool active;          // Indica si el NPC está actualmente activo/visible
	float speed;          // Velocidad actual de movimiento del NPC

	// ========================================================================
	// PARÁMETROS DE MOVIMIENTO Y COMPORTAMIENTO
	// ========================================================================
	int maxHeight;        // Altura máxima donde puede aparecer el NPC
	int maxSpeed;         // Velocidad máxima que puede alcanzar
	int sentit;           // Dirección del movimiento: 1 = derecha, -1 = izquierda
	float posicioInicialX; // Posición X inicial cuando el NPC reaparece

	// ========================================================================
	// CONSTRUCTOR DEL NPC
	// ========================================================================
	// Inicializa un NPC con todos sus parámetros necesarios
	// Parámetros:
	//   - texture: La textura/imagen del NPC
	//   - maxHeight_: Altura máxima de aparición
	//   - maxSpeed_: Velocidad máxima de movimiento
	//   - sentit_: Dirección (1 o -1)
	//   - posicioInicialX_: Posición inicial X
	NPC(const Texture& texture, int maxHeight_, int maxSpeed_, int sentit_, float posicioInicialX_)
		:sprite(texture),           // Inicializa el sprite con la textura
		active(false),              // Empieza inactivo
		speed(0),                   // Velocidad inicial es 0
		maxHeight(maxHeight_),      // Guarda la altura máxima
		maxSpeed(maxSpeed_),        // Guarda la velocidad máxima
		sentit(sentit_),            // Guarda la dirección
		posicioInicialX(posicioInicialX_) // Guarda la posición inicial
	{
		// Asegura que el sprite tenga la textura correcta
		sprite.setTexture(texture);
	}
};

// ============================================================================
// BLOQUE 4: PROTOTIPOS DE FUNCIONES
// ============================================================================
// Declaraciones de funciones que se implementan en otros archivos (.cpp)
// Estas funciones gestionan el comportamiento de NPCs y ramas

// updateNPC: Actualiza la posición y estado de un NPC en cada frame
// Parámetros:
//   - npc: Referencia al NPC que se va a actualizar
//   - dt: Delta time (tiempo transcurrido desde el último frame)
void updateNPC(NPC& npc, float dt);

// updateBranches: Actualiza las posiciones lógicas de las ramas del árbol
// Hace que las ramas "bajen" cuando el jugador tala y genera nuevas ramas arriba
// Parámetros:
//   - branchPositions: Array con las posiciones actuales de las ramas
//   - seed: Semilla para generar aleatoriedad en nuevas ramas
void updateBranches(Side branchPositions[], int seed);

// updateBranchSprites: Actualiza las posiciones visuales de los sprites de ramas
// Sincroniza los sprites con las posiciones lógicas almacenadas en branchPositions
// Parámetros:
//   - branchPositions: Array con las posiciones lógicas de las ramas
//   - branches: Array de sprites de ramas que se van a actualizar visualmente
void updateBranchSprites(Side branchPositions[], sf::Sprite branches[]);