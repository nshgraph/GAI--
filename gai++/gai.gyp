{
  'target_defaults': {
    'xcode_settings': {
      #todo compiler setting 'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',

      'SYMROOT': '<(DEPTH)/bin',
      'SDKROOT': 'macosx10.7',
      'MACOSX_DEPLOYMENT_TARGET': '10.6',
      'ARCHS': '$(ARCHS_STANDARD_64_BIT)',
      'VALID_ARCHS': [
        'i386',
        'x86_64',
      ],

      'USE_HEADERMAP': 'NO',

      #todo warning settings
      'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
      'GCC_TREAT_WARNINGS_AS_ERRORS': 'YES',

    },
    #todo win default settings

    'configurations': {
      'Debug': {
        'xcode_settings': {
          'GCC_OPTIMIZATION_LEVEL': '0'
        },
        'msbuild_settings': {
          'ClCompile': { 
            'Optimization': 'Disabled'
          }
        }
      },
      'Release': {
        'xcode_settings': {
          'GCC_OPTIMIZATION_LEVEL': 's'
        },
        'msbuild_settings': {
          'ClCompile': { 
            'Optimization': 'MaxSpeed'
          }
        }
      }
    }
  },
  'targets': [
    {
      'target_name': 'gai++',
      'type': 'static_library',
      'dependencies': [
      ],
      'include_dirs': [
        '../include',
        'src',
        'thirdparty/TinyThread++-1.1/source',
        'thirdparty/sqlite3',
      ],
      'sources': [
        'thirdparty/TinyThread++-1.1/source/fast_mutex.h',
        'thirdparty/TinyThread++-1.1/source/tinythread.h',
        'thirdparty/TinyThread++-1.1/source/tinythread.cpp',
        'thirdparty/sqlite3/sqlite3.h',
        'thirdparty/sqlite3/sqlite3.c',

        'src/ClientID.cpp',
        'src/ClientID.h',
        'src/DataStoreSqlite.h',
        'src/DataStoreSqlite.cpp',
        'src/DataStore.h',
        'src/Dispatcher.cpp',
        'src/Dispatcher.h',
        'src/GAI.cpp',
        'src/GAI.h',
        'src/GAIDefines.h',
        'src/Hit.cpp',
        'src/Hit.h',
        'src/HitBuilder.cpp',
        'src/HitBuilder.h',
        'src/HitStore.h',
        'src/HitType.h',
        'src/Model.cpp',
        'src/Model.h',
        'src/Timestamp.cpp',
        'src/Timestamp.h',
        'src/Tracker.h',
        'src/TrackerImpl.cpp',
        'src/TrackerImpl.h',
        'src/Transaction.cpp',
        'src/Transaction.h',
        'src/TransactionItem.cpp',
        'src/TransactionItem.h',
        'src/URLConnection.cpp',
        'src/URLConnection.h',
        'src/Platform.h'
      ],
      'conditions': [
         ['OS=="win"', {
          'defines': [
            'WIN32',
          ],
          'sources': [
            'src/win/Platform.cpp'
         ],
        }],
        ['OS=="mac"', {
          'include_dirs': [
            '/usr/local/include',
          ],
          'sources': [
            'src/mac/Platform.cpp'
          ],
        }],
      ],
    },
    {
      'target_name': 'gai++test',
      'type': 'executable',
      'dependencies': [
      ],
      'include_dirs': [
        '../include',
        'src',
        'thirdparty/TinyThread++-1.1/source',
        'thirdparty/sqlite3',
      ],
      'sources': [
        'unittests/main.cpp',
        'unittests/test_datastoresqlite.cpp',
        'unittests/test_dispatcher.cpp',
        'unittests/test_hit.cpp',
        'unittests/test_hitbuilder.cpp',
        'unittests/test_model.cpp',
        'unittests/test_trackerimpl.cpp',
        'unittests/test_utils.cpp',
        'unittests/test_gai.cpp',
        'unittests/test_urlconnection.cpp',
        
        'thirdparty/TinyThread++-1.1/source/fast_mutex.h',
        'thirdparty/TinyThread++-1.1/source/tinythread.h',
        'thirdparty/TinyThread++-1.1/source/tinythread.cpp',
        'thirdparty/sqlite3/sqlite3.h',
        'thirdparty/sqlite3/sqlite3.c',

        'src/ClientID.h',
        'src/ClientID.cpp',
        'src/DataStoreSqlite.h',
        'src/DataStoreSqlite.cpp',
        'src/DataStore.h',
        'src/Dispatcher.h',
        'src/Dispatcher.cpp',
        'src/GAI.cpp',
        'src/Hit.h',
        'src/Hit.cpp',
        'src/HitBuilder.h',
        'src/HitBuilder.cpp',
        'src/HitStore.h',
        'src/Model.h',
        'src/Model.cpp',
        'src/Timestamp.cpp',
        'src/TrackerImpl.h',
        'src/TrackerImpl.cpp',
        'src/Transaction.h',
        'src/Transaction.cpp',
        'src/TransactionItem.h',
        'src/TransactionItem.cpp',
        'src/URLConnection.h',
        'src/URLConnection.cpp',
      ],
      'conditions': [
        ['OS=="win"', {
          'defines': [
            'WIN32',
          ],
          'sources': [
            'src/win/Platform.cpp'
          ],
          'link_settings': {
            'libraries' : [
              'gtest.lib',
              'libevent.lib',
            ],
          },
        }],
        ['OS=="mac"', {
          'mac_bundle': 1,
          'include_dirs': [
            '/usr/local/include',
          ],
          'sources': [
            'src/mac/Platform.cpp'
          ],
          'xcode_settings': {
            'LIBRARY_SEARCH_PATHS': [
              '/usr/local/lib'
            ],
            'FRAMEWORK_SEARCH_PATHS': [
              '/Library/Frameworks'
            ],
          },
          'link_settings': {
            'libraries' : [
              'gtest.framework',
              'libevent.dylib',
              'ApplicationServices.framework'
            ],
          },
        }],
      ],
    },
  ],
}
