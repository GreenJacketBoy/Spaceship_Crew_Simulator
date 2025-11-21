#include "tests.h"
#include "Model/crew_room_linker_model.h"
// Yeah the tests are mostly AI generated, I don't like writing tests and this
// is a side project which doesn't even aim to be used by anyoneso I'm not apologizing

// Mock crewMember and room structures
typedef struct crewMember
{
    int id;
} crewMember;

typedef struct room
{
    int id;
} room;

// Test for buildCrewRoomLink
void testBuildCrewRoomLink()
{
    crewMember mockCrewMember = {1};
    room mockCurrentRoom = {2};
    room mockDestinationRoom = {3};

    crewRoomLink *link = buildCrewRoomLink(&mockCrewMember, &mockCurrentRoom, &mockDestinationRoom);

    assert(link != NULL);
    assert(link->crewMember == &mockCrewMember);
    assert(link->currentRoom == &mockCurrentRoom);
    assert(link->destinationRoom == &mockDestinationRoom);

    free(link);
    printf("testBuildCrewRoomLink: PASSED\n");
}

// Test for addToCrewRoomLinker
void testAddToCrewRoomLinker() {
    crewRoomLink **mockCrewRoomLinker = NULL;
    size_t mockCrewRoomLinkerSize = 0;
    crewMember mockCrewMember = {1};
    room mockCurrentRoom = {2};
    room mockDestinationRoom = {3};

    crewRoomLink *link = buildCrewRoomLink(&mockCrewMember, &mockCurrentRoom, &mockDestinationRoom);

    int result = addToCrewRoomLinker(&mockCrewRoomLinker, &mockCrewRoomLinkerSize, link);

    assert(result == 0);
    assert(mockCrewRoomLinkerSize == 1);
    assert(mockCrewRoomLinker != NULL);
    assert(mockCrewRoomLinker[0] == link);

    printf("testAddToCrewRoomLinker: PASSED\n");
}

// Test for removeFromCrewRoomLinker
void testRemoveFromCrewRoomLinker()
{
    crewRoomLink **mockCrewRoomLinker = NULL;
    size_t mockCrewRoomLinkerSize = 0;
    crewMember mockCrewMember = {1};
    room mockCurrentRoom = {2};
    room mockDestinationRoom = {3};

    crewRoomLink *link = buildCrewRoomLink(&mockCrewMember, &mockCurrentRoom, &mockDestinationRoom);

    addToCrewRoomLinker(&mockCrewRoomLinker, &mockCrewRoomLinkerSize, link);

    int result = removeFromCrewRoomLinker(&mockCrewRoomLinker, &mockCrewRoomLinkerSize, link);

    assert(result == 0);
    assert(mockCrewRoomLinkerSize == 0);

    free(link);
    printf("testRemoveFromCrewRoomLinker: PASSED\n");
}

// Test for getCrewRoomLinkByCrewMember
void testGetCrewRoomLinkByCrewMember()
{
    crewRoomLink **mockCrewRoomLinker = NULL;
    size_t mockCrewRoomLinkerSize = 0;
    crewMember mockCrewMember = {1};
    room mockCurrentRoom = {2};
    room mockDestinationRoom = {3};

    crewRoomLink *link = buildCrewRoomLink(&mockCrewMember, &mockCurrentRoom, &mockDestinationRoom);

    addToCrewRoomLinker(&mockCrewRoomLinker, &mockCrewRoomLinkerSize, link);

    crewRoomLink *foundLink = getCrewRoomLinkByCrewMember(mockCrewRoomLinker, mockCrewRoomLinkerSize, &mockCrewMember);

    assert(foundLink != NULL);
    assert(foundLink == link);

    printf("testGetCrewRoomLinkByCrewMember: PASSED\n");
}

int launchAllTests()
{
    testBuildCrewRoomLink();
    testAddToCrewRoomLinker();
    testRemoveFromCrewRoomLinker();
    testGetCrewRoomLinkByCrewMember();

    printf("All tests PASSED\n");
    return 0;
}

int main()
{
    launchAllTests();
    return 0;
}