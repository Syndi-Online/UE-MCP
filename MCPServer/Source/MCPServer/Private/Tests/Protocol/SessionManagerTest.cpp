// Copyright Epic Games, Inc. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Protocol/MCPSessionManager.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSessionManagerCreateTest,
	"MCPServer.Protocol.SessionManager.Create",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSessionManagerCreateTest::RunTest(const FString& Parameters)
{
	FMCPSessionManager Manager;
	FString SessionId = Manager.CreateSession();
	TestTrue(TEXT("Session ID should not be empty"), !SessionId.IsEmpty());
	TestTrue(TEXT("Session should be valid"), Manager.IsValidSession(SessionId));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSessionManagerRemoveTest,
	"MCPServer.Protocol.SessionManager.Remove",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSessionManagerRemoveTest::RunTest(const FString& Parameters)
{
	FMCPSessionManager Manager;
	FString SessionId = Manager.CreateSession();
	TestTrue(TEXT("Session valid before remove"), Manager.IsValidSession(SessionId));
	Manager.RemoveSession(SessionId);
	TestFalse(TEXT("Session invalid after remove"), Manager.IsValidSession(SessionId));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSessionManagerInvalidTest,
	"MCPServer.Protocol.SessionManager.Invalid",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSessionManagerInvalidTest::RunTest(const FString& Parameters)
{
	FMCPSessionManager Manager;
	TestFalse(TEXT("Random ID should be invalid"), Manager.IsValidSession(TEXT("nonexistent-id")));
	TestFalse(TEXT("Empty ID should be invalid"), Manager.IsValidSession(TEXT("")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSessionManagerMultipleTest,
	"MCPServer.Protocol.SessionManager.Multiple",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSessionManagerMultipleTest::RunTest(const FString& Parameters)
{
	FMCPSessionManager Manager;
	FString Id1 = Manager.CreateSession();
	FString Id2 = Manager.CreateSession();
	FString Id3 = Manager.CreateSession();

	TestTrue(TEXT("All three sessions should be valid"), Manager.IsValidSession(Id1) && Manager.IsValidSession(Id2) && Manager.IsValidSession(Id3));

	Manager.RemoveSession(Id2);
	TestTrue(TEXT("Session 1 still valid"), Manager.IsValidSession(Id1));
	TestFalse(TEXT("Session 2 removed"), Manager.IsValidSession(Id2));
	TestTrue(TEXT("Session 3 still valid"), Manager.IsValidSession(Id3));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSessionManagerUniqueIdsTest,
	"MCPServer.Protocol.SessionManager.UniqueIds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FSessionManagerUniqueIdsTest::RunTest(const FString& Parameters)
{
	FMCPSessionManager Manager;
	TSet<FString> Ids;
	for (int32 i = 0; i < 100; ++i)
	{
		FString Id = Manager.CreateSession();
		TestFalse(TEXT("Each session ID should be unique"), Ids.Contains(Id));
		Ids.Add(Id);
	}
	TestEqual(TEXT("Should have 100 unique IDs"), Ids.Num(), 100);
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
