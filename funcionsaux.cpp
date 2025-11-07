#include "funcionsaux.h"

// ============================================================================
// BLOQUE 1: IMPLEMENTACIÓN DE FUNCIONES AUXILIARES
// ============================================================================
// Este archivo contiene la implementación de las funciones declaradas en
// funcionsaux.h. Gestiona el comportamiento de NPCs y el sistema de ramas.

// ============================================================================
// BLOQUE 2: FUNCIÓN updateNPC - ACTUALIZACIÓN DE ENTIDADES MÓVILES
// ============================================================================
// Esta función controla el ciclo de vida completo de un NPC (abeja o nube):
// 1. Activación inicial con posición y velocidad aleatorias
// 2. Movimiento continuo mientras está activo
// 3. Desactivación cuando sale de la pantalla
//
// Parámetros:
//   - npc: Referencia al NPC que se va a actualizar
//   - dt: Delta time (tiempo transcurrido desde el último frame en segundos)
void updateNPC(NPC& npc, float dt) {

	// ========================================================================
	// BLOQUE 2A: ACTIVACIÓN DEL NPC (SI ESTÁ INACTIVO)
	// ========================================================================
	// Cuando el NPC no está activo, se le asignan nuevos valores aleatorios
	// y se coloca en su posición inicial para comenzar a moverse
	if (!npc.active) {
		// Genera una velocidad aleatoria entre 0 y maxSpeed
		// Se multiplica por 'sentit' para determinar la dirección (1 o -1)
		npc.speed = (rand() % npc.maxSpeed) * npc.sentit;

		// Genera una altura aleatoria entre 0 y maxHeight
		// Esto hace que cada vez aparezca a diferentes alturas
		float height = rand() % npc.maxHeight;

		// Coloca el NPC en su posición inicial X y en la altura aleatoria Y
		npc.sprite.setPosition({ npc.posicioInicialX, height });

		// Activa el NPC para que comience a moverse
		npc.active = true;
	}
	// ========================================================================
	// BLOQUE 2B: ACTUALIZACIÓN DEL MOVIMIENTO (SI ESTÁ ACTIVO)
	// ========================================================================
	// Cuando el NPC está activo, actualiza su posición horizontal
	else {
		// Mueve el NPC horizontalmente según su velocidad y el tiempo transcurrido
		// La fórmula es: nueva_posición = posición_actual + velocidad * delta_time
		// Esto garantiza movimiento suave independiente de los FPS
		npc.sprite.setPosition({
			npc.sprite.getPosition().x + npc.speed * dt,  // Nueva posición X
			npc.sprite.getPosition().y                     // Mantiene posición Y
			});

		// ====================================================================
		// BLOQUE 2C: DESACTIVACIÓN CUANDO SALE DE PANTALLA
		// ====================================================================
		// Comprueba si el NPC ha salido de los límites visibles
		// Si está muy a la izquierda (< -200) o muy a la derecha (> 2000)
		// se desactiva para que pueda reaparecer en la posición inicial
		if (npc.sprite.getPosition().x < -200 || npc.sprite.getPosition().x > 2000)
			npc.active = false;
	}
}

// ============================================================================
// BLOQUE 3: FUNCIÓN updateBranches - SISTEMA DE RAMAS DEL ÁRBOL
// ============================================================================
// Esta función gestiona la lógica de las ramas cuando el jugador tala:
// 1. Desplaza todas las ramas hacia abajo (simulando que caen)
// 2. Genera una nueva rama aleatoria en la parte superior
//
// Parámetros:
//   - branchPositions: Array que contiene las posiciones de las 6 ramas
//   - seed: Semilla adicional para la generación aleatoria (suele ser el score)
void updateBranches(Side branchPositions[], int seed) {

	// ========================================================================
	// BLOQUE 3A: DESPLAZAMIENTO DE RAMAS HACIA ABAJO
	// ========================================================================
	// Recorre el array desde la penúltima rama (índice 4) hasta la primera
	// y mueve cada rama a la posición de la rama superior
	// Esto simula que todas las ramas "caen" una posición cuando el jugador tala
	//
	// Ejemplo: si teníamos [LEFT, NONE, RIGHT, LEFT, NONE, RIGHT]
	//          quedará     [???, LEFT, NONE, RIGHT, LEFT, NONE]
	//          donde ??? será la nueva rama que generamos a continuación
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
		branchPositions[j] = branchPositions[j - 1];

	// ========================================================================
	// BLOQUE 3B: GENERACIÓN DE NUEVA RAMA EN LA CIMA
	// ========================================================================
	// Genera un número aleatorio entre 0 y 4 usando rand() más la semilla
	// La semilla (normalmente el score) hace que los patrones cambien con el juego
	int r = (rand() + seed) % 5;

	// Según el número aleatorio, decide dónde colocar la nueva rama:
	// - 0: Rama a la izquierda (20% probabilidad)
	// - 1: Rama a la derecha (20% probabilidad)
	// - 2, 3, 4: Sin rama (60% probabilidad)
	// Esta distribución hace que haya más espacios vacíos que ramas
	switch (r) {
	case 0:
		branchPositions[0] = Side::LEFT;   // Nueva rama en el lado izquierdo
		break;
	case 1:
		branchPositions[0] = Side::RIGHT;  // Nueva rama en el lado derecho
		break;
	default:
		branchPositions[0] = Side::NONE;   // Sin rama (espacio vacío)
		break;
	}
}

// ============================================================================
// BLOQUE 4: FUNCIÓN updateBranchSprites - RENDERIZADO VISUAL DE RAMAS
// ============================================================================
// Esta función sincroniza los sprites visuales con las posiciones lógicas
// Convierte los datos abstractos (LEFT, RIGHT, NONE) en posiciones y
// rotaciones reales en pantalla para los sprites de las ramas
//
// Parámetros:
//   - branchPositions: Array con las posiciones lógicas de las ramas
//   - branches: Array de sprites que se van a actualizar visualmente
void updateBranchSprites(Side branchPositions[], Sprite branches[]) {

	// ========================================================================
	// BLOQUE 4A: RECORRIDO Y ACTUALIZACIÓN DE CADA RAMA
	// ========================================================================
	// Recorre todas las ramas del árbol y actualiza su posición visual
	for (int i = 0; i < NUM_BRANCHES; i++) {

		// Calcula la altura Y de cada rama: cada rama está separada 150 píxeles
		// Rama 0 (arriba): Y = 0
		// Rama 1: Y = 150
		// Rama 2: Y = 300
		// ...
		// Rama 5 (abajo, cerca del jugador): Y = 750
		float height = i * 150;

		// ====================================================================
		// BLOQUE 4B: RAMA EN EL LADO IZQUIERDO
		// ====================================================================
		// Si la rama está en el lado izquierdo del árbol:
		if (branchPositions[i] == Side::LEFT) {
			branches[i].setPosition({ 610, height });        // Posiciona en X=610 (izquierda)
			branches[i].setRotation(sf::degrees(180.0f));    // Rota 180° para que apunte a la izquierda
		}
		// ====================================================================
		// BLOQUE 4C: RAMA EN EL LADO DERECHO
		// ====================================================================
		// Si la rama está en el lado derecho del árbol:
		else if (branchPositions[i] == Side::RIGHT) {
			branches[i].setPosition({ 1330, height });       // Posiciona en X=1330 (derecha)
			branches[i].setRotation(sf::degrees(0.0f));      // Sin rotación (orientación original)
		}
		// ====================================================================
		// BLOQUE 4D: SIN RAMA (ESPACIO VACÍO)
		// ====================================================================
		// Si no hay rama en esta posición (NONE):
		else {
			branches[i].setPosition({ -2000, height });      // La oculta fuera de la pantalla
		}
	}
}