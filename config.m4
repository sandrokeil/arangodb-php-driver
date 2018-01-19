PHP_ARG_ENABLE(arangodb,
    [Whether to enable the "arangodb" extension],
    [  --enable-arangodb      Enable "arangodb" extension support])

if test $PHP_ARANGODB != "no"; then
    for i in /usr /usr/local; do
      if test -f $i/include/velocypack/vpack.h; then
        VELOCYPACK_INCLUDE_DIR=$i
        AC_MSG_RESULT(found velocypack includes in $i)
        break
      fi
    done

    for i in /usr /usr/local; do
      if test -f $i/include/velocypack/vpack.h; then
        FUERTE_INCLUDE_DIR=$i
        AC_MSG_RESULT(found fuerte includes in $i)
        break
      fi
    done

    for i in /usr /usr/local; do
      if test -f $i/lib/libvelocypack.a; then
        VELOCYPACK_LIB_DIR=$i
        AC_MSG_RESULT(found velocypack lib in $i)
        break
      fi
    done

    for i in /usr /usr/local; do
      if test -f $i/lib/libfuerte.a; then
        FUERTE_LIB_DIR=$i
        AC_MSG_RESULT(found fuerte lib in $i)
        break
      fi
    done

    BOOST_THREAD=boost_thread

    for i in /usr /usr/local; do
      if test -f $i/lib/libboost_thread-mt.so; then
        BOOST_THREAD=boost_thread-mt
        break
      fi
    done

    PHP_ADD_INCLUDE($VELOCYPACK_INCLUDE_DIR)
    PHP_ADD_INCLUDE($FUERTE_INCLUDE_DIR)

    CXXFLAGS="-std=c++11"
    PHP_REQUIRE_CXX()

    PHP_SUBST(ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(velocypack, $VELOCYPACK_LIB_DIR, ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(fuerte, $FUERTE_LIB_DIR, ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY(curl, 1, ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY(boost_system, 1, ARANGODB_SHARED_LIBADD)
    PHP_ADD_LIBRARY($BOOST_THREAD, 1, ARANGODB_SHARED_LIBADD)
    PHP_NEW_EXTENSION(arangodb, php_arangodb.cpp src/vpack.cpp src/request.cpp src/connection.cpp, $ext_shared)
fi
