#ifndef GLOBAL_H
#define GLOBAL_H

#include "beem.h"
#include "system_info.h"
#include <WString.h>
#include "event.h"
extern Beem beem;

using namespace std;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#endif //GLOBAL_H
