/*
 *  Copyright 2015 Igor K. aka nonametr
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef COMMON_H
#define COMMON_H

#define ASSERT_WITH_CODE_5(cond, ...) \
    if(!(cond)) \
{ \
    traceError2(GET_1TH_ARG(__VA_ARGS__), GET_2TH_ARG(__VA_ARGS__), GET_3TH_ARG(__VA_ARGS__)); \
    GET_4TH_ARG(__VA_ARGS__); \
}

#define ASSERT_WITH_CODE_4(cond, ...) \
    if(!(cond)) \
{ \
    traceError2(GET_1TH_ARG(__VA_ARGS__), GET_2TH_ARG(__VA_ARGS__)); \
    GET_3TH_ARG(__VA_ARGS__); \
}

#define ASSERT_WITH_CODE_3(cond, msg, code) \
        if(!(cond)) \
{ \
        perror(msg); \
        code; \
}

#define ASSERT_WITH_CODE_2(cond, code) \
        if(!(cond)) \
{ \
        code; \
}

#define ASSERT_WITH_CODE_1(cond) \
        if(!(cond)) \
{ \
        perror("ASSERTION FAILED! Program terminated"); \
        exit(1); \
}

#define GET_1TH_ARG(arg1, ...) arg1
#define GET_2TH_ARG(arg1, arg2, ...) arg2
#define GET_3TH_ARG(arg1, arg2, arg3, ...) arg3
#define GET_4TH_ARG(arg1, arg2, arg3, arg4, ...) arg4
#define GET_5TH_ARG(arg1, arg2, arg3, arg4, arg5, ...) arg5
#define GET_6TH_ARG(arg1, arg2, arg3, arg4, arg5, arg6,...) arg6
#define GET_7TH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, arg7,...) arg7
#define GET_8TH_ARG(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, ...) arg8
#define ASSERT_WITH_CODE_MACRO_CHOOSER(...) GET_8TH_ARG(__VA_ARGS__, ASSERT_WITH_CODE_UNKNOWN, ASSERT_WITH_CODE_UNKNOWN, ASSERT_WITH_CODE_5, ASSERT_WITH_CODE_4, \
                             ASSERT_WITH_CODE_3, ASSERT_WITH_CODE_2, ASSERT_WITH_CODE_1, )

#define ASSERT_WITH_CODE(...) ASSERT_WITH_CODE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#endif // COMMON_H

