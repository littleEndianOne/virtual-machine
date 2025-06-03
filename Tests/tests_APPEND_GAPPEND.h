#pragma once

char* test_APPEND_Local_StrLit();
char* test_APPEND_Local_StrRef();
char* test_APPEND_Local_Float();
char* test_APPEND_Local_Int();

char* test_APPEND_EmptyStrRef();
char* test_APPEND_ToEmptyStr();
char* test_APPEND_AppendEmpty();

char* test_APPEND_BuildString();
char* test_APPEND_Global_StrLit();

char* test_APPEND_ToMinAllocLength();
char* test_APPEND_AppendToFull();
char* test_APPEND_CauseOverflow();


void RunSet_APPEND();
