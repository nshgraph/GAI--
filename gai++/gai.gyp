{
  'target_defaults': {
    'xcode_settings': {
      #todo compiler setting 'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',

      'SYMROOT': '<(DEPTH)/bin',
      'MACOSX_DEPLOYMENT_TARGET': '10.6',
      'ARCHS': '$(ARCHS_STANDARD_64_BIT)',
      'VALID_ARCHS': [
        'i386',
        'x86_64',
      ],

      'USE_HEADERMAP': 'NO',

      #todo warning settings
      'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES',
      'GCC_TREAT_WARNINGS_AS_ERRORS': 'NO',

    },
    'msvs_configuration_attributes': {
      'UseOfMFC': '2', # 0=false 1=Static 2=Dynamic
      'CharacterSet': '1', # Unicode
      'OutputDirectory': '<(DEPTH)/bin/$(ProjectName)/$(Configuration)',
      'IntermediateDirectory': '<(DEPTH)/bin/$(ProjectName)/obj/$(Configuration)',
    },

    'configurations': {
      'Debug': {
        'xcode_settings': {
          'GCC_OPTIMIZATION_LEVEL': '0'
        },
        'msbuild_settings': {
          'ClCompile': { 
            'Optimization': 'Disabled',
            'RuntimeLibrary': 'MultiThreadedDebugDLL',
          }
        }
      },
      'Debug_x64': {
        'inherit_from': [ 'Debug', ],
        'msvs_configuration_platform': 'x64',
      },
      'Release': {
        'xcode_settings': {
          'GCC_OPTIMIZATION_LEVEL': 's'
        },
        'msbuild_settings': {
          'ClCompile': { 
            'Optimization': 'MaxSpeed',
            'RuntimeLibrary': 'MultiThreadedDLL',
          }
        }
      },
      'Release_x64': {
        'inherit_from': [ 'Release', ],
        'msvs_configuration_platform': 'x64',
      },
    }
  },
  'targets': [
  {
      'target_name': 'libevent',
      'type': 'static_library',
      'dependencies': [
      ],
      'include_dirs': [
        'thirdparty/libevent-2.0.21-stable',
        'thirdparty/libevent-2.0.21-stable/compat',
      ],
      'sources': [
        'thirdparty/libevent-2.0.21-stable/event.h',
        'thirdparty/libevent-2.0.21-stable/event.c',
        'thirdparty/libevent-2.0.21-stable/buffer.c',
        'thirdparty/libevent-2.0.21-stable/bufferevent.c',
        'thirdparty/libevent-2.0.21-stable/bufferevent_filter.c',
        'thirdparty/libevent-2.0.21-stable/bufferevent_sock.c',
        'thirdparty/libevent-2.0.21-stable/bufferevent_pair.c',
        'thirdparty/libevent-2.0.21-stable/bufferevent_ratelim.c',
        'thirdparty/libevent-2.0.21-stable/listener.c',
        'thirdparty/libevent-2.0.21-stable/evmap.c',
        'thirdparty/libevent-2.0.21-stable/log.c',
        'thirdparty/libevent-2.0.21-stable/evutil.h',
        'thirdparty/libevent-2.0.21-stable/evutil.c',
        'thirdparty/libevent-2.0.21-stable/evutil_rand.c',
        'thirdparty/libevent-2.0.21-stable/evthread.c',
        'thirdparty/libevent-2.0.21-stable/strlcpy.c',
        'thirdparty/libevent-2.0.21-stable/signal.c',

        'thirdparty/libevent-2.0.21-stable/event_tagging.c',
        'thirdparty/libevent-2.0.21-stable/http.c',
        'thirdparty/libevent-2.0.21-stable/evdns.h',
        'thirdparty/libevent-2.0.21-stable/evdns.c',
        'thirdparty/libevent-2.0.21-stable/evrpc.h',
        'thirdparty/libevent-2.0.21-stable/evrpc.c',
      ],
      'conditions': [
        ['OS=="win"', {
          'defines': [
            'WIN32',
            'HAVE_CONFIG_H',
          ],
          'include_dirs': [
            'thirdparty/libevent-2.0.21-stable/WIN32-Code',
            'thirdparty/libevent-2.0.21-stable/include',
          ],
          'sources': [
            'thirdparty/libevent-2.0.21-stable/win32select.c',
            'thirdparty/libevent-2.0.21-stable/evthread_win32.c',
            'thirdparty/libevent-2.0.21-stable/buffer_iocp.c',
            'thirdparty/libevent-2.0.21-stable/event_iocp.c',
            'thirdparty/libevent-2.0.21-stable/bufferevent_async.c',
          ],
          'link_settings': {
            'libraries' : [
              'Advapi32.lib'
            ],
          },
          'msbuild_settings': {
            'Link': {
              'IgnoreSpecificDefaultLibraries': [
                'uafxcwd.lib',
                'libcmtd.lib',
                'libcmt.lib',
              ],
              'AdditionalLibraryDirectories': [
              ],
            },
          },

          'copies': [
            {
              'destination' : 'thirdparty/libevent-2.0.21-stable/include/event2',
              'files' : [
                'thirdparty/libevent-2.0.21-stable/WIN32-Code/event2/event-config.h',
              ]
            }
          ]
        }],
        ['OS=="mac"', {
          'include_dirs': [
            '/usr/local/include',
            'thirdparty/libevent-2.0.21-stable/MacOSX-Code/',
            'thirdparty/libevent-2.0.21-stable/include',
          ],
          'sources': [
            'thirdparty/libevent-2.0.21-stable/kqueue.c',
            'thirdparty/libevent-2.0.21-stable/poll.c',
            'thirdparty/libevent-2.0.21-stable/select.c',

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
            ],
          },
          'copies': [
            {
              'destination' : 'thirdparty/libevent-2.0.21-stable/include/event2',
              'files' : [
                'thirdparty/libevent-2.0.21-stable/MacOSX-Code/event2/event-config.h',
              ]
            }
          ]
        }],
      ],
    },
    {
      'target_name': 'gai++',
      'type': 'static_library',
      'dependencies': [
        'libevent',
      ],
      'include_dirs': [
        'include',
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

        'include/gai++/GAI.h',
        'include/gai++/Tracker.h',
        'include/gai++/Transaction.h',
        'include/gai++/TransactionItem.h',

        'src/ClientID.cpp',
        'src/ClientID.h',
        'src/DataStoreSqlite.h',
        'src/DataStoreSqlite.cpp',
        'src/DataStore.h',
        'src/DebugPrint.h',
        'src/Dispatcher.cpp',
        'src/Dispatcher.h',
        'src/GAI.cpp',
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
        'src/TrackerImpl.cpp',
        'src/TrackerImpl.h',
        'src/Transaction.cpp',
        'src/TransactionItem.cpp',
        'src/URLBuilder.cpp',
        'src/URLBuilder.h',
        'src/URLConnection.cpp',
        'src/URLConnection.h',
        'src/Platform.h'
      ],
      'conditions': [
         ['OS=="win"', {
          'defines': [
            'WIN32',
          ],
          'include_dirs': [
            'thirdparty/libevent-2.0.21-stable/WIN32-Code/',
            'thirdparty/libevent-2.0.21-stable/include',
          ],
          'sources': [
            'src/win/Platform.cpp'
         ],
          'link_settings': {
            'libraries' : [
              'Rpcrt4.lib',
              'User32.lib'
            ],
          },
          'msbuild_settings': {
            'Link': {
              'AdditionalLibraryDirectories': [
                '../lib/',
              ],
            },
          },
        }],
        ['OS=="mac"', {
          'include_dirs': [
            'thirdparty/libevent-2.0.21-stable/MacOSX-Code/',
            'thirdparty/libevent-2.0.21-stable/include',
          ],
          'sources': [
            'src/mac/Platform.cpp'
          ],
          'link_settings': {
            'libraries' : [
              'libevent.a',
            ],
          }
        }],
      ],
    },
    {
      'target_name': 'gai++test',
      'type': 'executable',
      'dependencies': [
        'gai++',
        'libevent'
      ],
      'include_dirs': [
        'include',
        'src',
        'thirdparty/TinyThread++-1.1/source',
        'thirdparty/sqlite3',
        'thirdparty/gtest-1.7.0/include',
        'thirdparty/gtest-1.7.0/',
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

        'thirdparty/gtest-1.7.0/include/gtest/gtest.h',
        'thirdparty/gtest-1.7.0/src/gtest-all.cc'
      ],
      'conditions': [
        ['OS=="win"', {
          'defines': [
            'WIN32',
          ],
          'include_dirs': [
            'thirdparty/libevent-2.0.21-stable/WIN32-Code/',
            'thirdparty/libevent-2.0.21-stable/include',
          ],
          'sources': [
            'src/win/Platform.cpp'
          ],
          'configurations': {
            'Debug': {
              'defines': [
                '_DEBUG',
              ],
              'msbuild_settings': {
                'ClCompile': {
                  'DebugInformationFormat': 'EditAndContinue',
                  'Optimization': 'Disabled',
                  'RuntimeLibrary': 'MultiThreadedDebugDLL',
                  'MinimalRebuild': 'true',
                  'BasicRuntimeChecks': 'Default',
                },
                'Link': {
                  'SubSystem': 'Console',
                  'GenerateDebugInformation': 'true',
                  'IgnoreSpecificDefaultLibraries': [
                    'uafxcwd.lib',
                    'libcmtd.lib',
                    'libcmt.lib',
                  ],
                  'AdditionalLibraryDirectories': [
                    '../lib/',
                  ],
                  'AdditionalDependencies': [
                    'ws2_32.lib',
                    'Rpcrt4.lib',
                  ],
                }, 
              },
            }, # end of Debug configuration
            'Release': {
              'msbuild_settings': {
                'Link': {
                  'SubSystem': 'Console',
                  'IgnoreSpecificDefaultLibraries': [
                    'uafxcwd.lib',
                    'libcmtd.lib',
                    'libcmt.lib',
                  ],
                  'AdditionalLibraryDirectories': [
                    '../lib/',
                  ],
                  'AdditionalDependencies': [
                    'ws2_32.lib',
                    'Rpcrt4.lib',
                  ],
                }, 
              },
            } # end of Release configuration
          },
        }
        ],
        ['OS=="mac"', {
          'mac_bundle': 1,
          'include_dirs': [
            '/usr/local/include',
            'thirdparty/libevent-2.0.21-stable/MacOSX-Code/',
            'thirdparty/libevent-2.0.21-stable/include',
          ],
          'sources': [
            'src/mac/Platform.cpp'
          ],
          'xcode_settings': {
            'LIBRARY_SEARCH_PATHS': [
              '/usr/local/lib',
              '../lib'
            ],
            'FRAMEWORK_SEARCH_PATHS': [
              '/Library/Frameworks'
            ],
          },
          'link_settings': {
            'libraries' : [
              'libevent.a',
              'ApplicationServices.framework'
            ],
          },
        }],
      ],
    },
  ],
}
