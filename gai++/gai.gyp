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
  },
  'targets': [
    {
      'target_name': 'gai++',
      'type': 'static_library',
      'dependencies': [
      ],
      'include_dirs': [
        'src',
      ],
      'sources': [
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
        'src/HitType.h',
        'src/Model.cpp',
        'src/Model.h',
        'src/ReachabilityChecker.cpp',
        'src/ReachabilityChecker.h',
        'src/ReachabilityDelegate.h',
        'src/RequestBuilder.cpp',
        'src/RequestBuilder.h',
        'src/Tracker.h',
        'src/TrackerImpl.cpp',
        'src/TrackerImpl.h',
        'src/Transaction.cpp',
        'src/Transaction.h',
        'src/TransactionItem.cpp',
        'src/TransactionItem.h',
        'src/URLConnection.cpp',
        'src/URLConnection.h',
      ],
      'conditions': [
        ['OS=="win"', {
          'configurations': {
            'Debug': {
              #todo symbol/optimisation settings
            },
            'Release': {
              #todo symbol/optimisation settings
            },
          },
        }],
        ['OS=="mac"', {
          'include_dirs': [
            '/usr/local/include',
          ],
          'configurations': {
            'Debug': {
              #todo symbol/optimisation settings
            },
            'Release': {
              #todo symbol/optimisation settings
            },
          },
        }],
      ],
    },
    {
      'target_name': 'gai++test',
      'type': 'executable',
      'dependencies': [
      ],
      'include_dirs': [
        'src',
      ],
      'sources': [
        'unittests/main.cpp',
        'unittests/test_datastoresqlite.cpp',
        'unittests/test_hit.cpp',

        'src/DataStoreSqlite.h',
        'src/DataStoreSqlite.cpp',
        'src/DataStore.h',
        'src/Hit.h',
        'src/Hit.cpp',
      ],
      'conditions': [
        ['OS=="win"', {
          'configurations': {
            'Debug': {
              #todo symbol/optimisation settings
            },
            'Release': {
              #todo symbol/optimisation settings
            },
          },
        }],
        ['OS=="mac"', {
          'mac_bundle': 1,
          'include_dirs': [
            '/usr/local/include',
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
              'libsqlite3.dylib',
              'libevent.dylib'
            ],
          },
          'configurations': {
            'Debug': {
              #todo symbol/optimisation settings
            },
            'Release': {
              #todo symbol/optimisation settings
            },
          },
        }],
      ],
    },
  ],
}
