address command 'svn update'
address command 'svn update' with output stem svn.
if svn.0 <> 2 then
do
  Say 'Unexpected svn output'
  do i=1 to svn.0
    say svn.i
  end
  exit
end
parse var svn.2 At revision level '.'
'rm -rf patchset/p????.patch'
do patchno=level to 1 by -1
  patchfn = './patchset/p' || right('0000' || patchno, 4) || '.patch'
  'svn log -r' patchno '>' patchfn
  'svn diff -r' patchno-1 || ':' || patchno '>>' patchfn
  Say 'Patch' patchno
End
