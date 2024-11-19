#ifndef IFB_COMMON_SCOPES_HPP
#define IFB_COMMON_SCOPES_HPP

#define ifb_internal static
#define ifb_local    static
#define ifb_global   static
#define ifb_api      extern "C" __declspec(dllexport)

#endif //IFB_COMMON_SCOPES_HPP