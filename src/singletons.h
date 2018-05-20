#pragma once

#include    "stdc_common.h"
#include    "miscutils.h"

#include    "taskexecutor.h"
#include    "recrouter.h"

int laft_init(void);

struct task_executor *common_executor(void);
struct recrouter *common_recrouter(void);
