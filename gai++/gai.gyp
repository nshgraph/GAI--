{
  'target_defaults': {
    'xcode_settings': {
      #todo compiler setting 'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',

      'SYMROOT': '<(DEPTH)/bin',
      'SDKROOT': 'macosx10.6',
      'MACOSX_DEPLOYMENT_TARGET': '10.6',
      'ARCHS': '$(ARCHS_STANDARD_32_BIT)',

      'USE_HEADERMAP': 'NO',

      'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
      'GCC_TREAT_WARNINGS_AS_ERRORS': 'YES',
      #todo warning settings
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
        'src/DataStore.cpp',
        'src/DataStore.h',
        'src/Dispatcher.cpp',
        'src/Dispatcher.h',
        'src/GAI.cpp',
        'src/GAI.h',
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
        'src/Tracker.cpp',
        'src/Tracker.h',
        'src/TrackerImpl.cpp',
        'src/TrackerImpl.h',
        'src/TrackerState.h',
        'src/Transaction.cpp',
        'src/Transaction.h',
        'src/Transaction_Item.cpp',
        'src/Transaction_Item.h',
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
        }, { # OS != "win",
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
