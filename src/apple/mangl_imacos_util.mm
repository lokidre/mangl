#include "../core/mangl_util.h"

#include "mangl_imacos_util.h"

namespace mangl {

String Uuid::generateString()
{
    String str{toString(NSUUID.UUID.UUIDString)};

	return str;
}

}  // namespace mangl

