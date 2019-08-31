project(
  'fell', 'c',
  version : '0.2.1'
)

inc = include_directories(
  'src',
  'src/util'
)

src = [
  'src/fell.c',
  'src/util/str.c',
  'src/util/vec.c'
]

executable('fell',
  src,
  include_directories: inc,
  install : true
)
