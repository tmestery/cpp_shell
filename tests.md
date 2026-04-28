# builtins
- ```help```
- ```pwd```
- ```cd ..```
- ```pwd```
- ```echo hello world```

# system commands
- ```ls```
- ```ls -la```
- ```cat <any file in repo>```
- ```mkdir testdir```
- ```ls```

# piping
- ```ls | grep src```
- ```ls -la | grep cpp```
- ```echo hello | cat```

# edge cases
### enter (key)
- // should do nothing

### Ctrl+C (should'nt kill shell)
- ```blah```        // should say command not found
- ```echo $HOME```  // should expand to your home dir