# builds the executable containing manual and move-generation tests
add_executable(chess_engine_tests
        main.cpp

        manual/src/ManualTestUtils.cpp
        manual/src/TestMoveValidation.cpp
        manual/src/TestPawnMovement.cpp
        manual/src/TestPieceMovement.cpp
        manual/src/TestKingSafety.cpp
        manual/src/TestCastling.cpp
        manual/src/TestEnPassant.cpp
        manual/src/TestPawnPromotion.cpp
        manual/src/TestMoveHistory.cpp
        manual/src/TestFenAndMoveCounters.cpp
        manual/src/TestGameStatus.cpp
        manual/src/RunManualTests.cpp

        move-generation/src/TestPerft.cpp
        move-generation/src/TestPseudoLegalMoveGeneration.cpp
        move-generation/src/TestLegalMoveFiltering.cpp
        move-generation/src/TestEnPassantMoveGeneration.cpp
        move-generation/src/TestCastlingMoveGeneration.cpp
        move-generation/src/TestPromotionMoveGeneration.cpp
        move-generation/src/RunMoveGenerationTests.cpp
)

# links the tests against the chess engine implementation
target_link_libraries(chess_engine_tests PRIVATE chess_core)

# adds the directories containing headers used by the test sources
target_include_directories(chess_engine_tests PRIVATE
        manual/include
        move-generation/include
)

# registers the test executable with CTest so it can be run through the test runner
add_test(NAME chess_engine_tests COMMAND chess_engine_tests)