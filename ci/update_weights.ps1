$size_t=select-string -path ".\bootstrap_weights.cpp" -Pattern "rai_bootstrap_weights_size"| foreach {$_.Line}
    
$old_size_t = "extern const size_t rai_bootstrap_weights_size;"

$char=select-string -path ".\bootstrap_weights.cpp" -Pattern "char rai_bootstrap_weights"| foreach {$_.Line}

$old_char = [regex]::Escape("extern const char rai_bootstrap_weights[];") 

select-string -path "..\rai\node\node.cpp" -Pattern $old_size_t -SimpleMatch | %{$curpath=$_.path; (get-content $curpath -Raw) -replace $old_char, $char| Out-File $curpath}

select-string -path "..\rai\node\node.cpp" -Pattern $old_size_t -SimpleMatch | %{$curpath=$_.path; (get-content $curpath -Raw) -replace $old_size_t, $size_t| Out-File $curpath}