//
// Created by devrupamsahoo on 10-11-2026.
//

#include "../include/RunEvaluationTests.h"
#include "../include/TestKingSafety.h"
#include "../include/TestEvaluation.h"
#include "../include/TestEvaluationIntegration.h"
#include "../include/TestGamePhase.h"
#include "../include/TestKingAttackers.h"
#include "../include/TestMaterialAndPSTs.h"
#include "../include/TestMobility.h"
#include "../include/TestPawnStructure.h"
#include "../include/TestPieceBonuses.h"


namespace EvaluationTests {
    void runEvaluationTests() {
        testMaterialAdvantage();
        testSideToMovePerspective();
        testBishopPairBonus();
        testRookOpenFileBonus();
        testPawnStructureTerms();
        testKingSafetyTerms();
        runMaterialAndPieceSquareTablesTests();
        runGamePhaseTests();
        runPieceBonusesTests();
        runPawnStructureTests();
        runKingSafetyTests();
        runKingAttackersTests();
        runMobilityTests();
        runEvaluationIntegrationTests();
    }
} // namespace EvaluationTests
