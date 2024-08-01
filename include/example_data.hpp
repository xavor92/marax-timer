// captured example data for testing
const char *example_data = "C1.10,115,O26,098,0000,0,0\n\
C1.10,115,O26,098,0000,0,0\n\
C1.10,115,O26,098,0000,0,0\n\
C1.10,115,O26,098,0000,0,0\n\
C1.10,115,O26,097,0000,0,0\n\
C1.10,115,O26,097,0000,0,0\n\
C1.10,115,O26,097,0000,0,0\n\
C1.10,115,O26,097,0000,0,0\n\
C1.10,114,O26,097,0000,0,0\n\
C1.10,114,O26,097,0000,0,0\n\
C1.10,114,O26,097,0000,0,0\n\
C1.10,114,O26,097,0000,0,0\n\
C1.10,113,O26,097,0000,0,0\n\
C1.10,113,O26,097,0000,0,0\n\
C1.10,113,O26,097,0000,0,0\n\
C1.10,113,O26,097,0000,0,0\n\
C1.10,112,O26,097,0000,0,0\n\
C1.10,112,O26,097,0000,0,0\n\
C1.10,112,O26,097,0000,0,0\n\
C1.10,112,O26,097,0000,0,0\n\
C1.10,111,O26,097,0000,0,0\n\
C1.10,111,128,098,0000,1,0\n\
C1.10,111,128,098,0000,1,0\n\
C1.10,111,128,098,0000,1,0\n\
C1.10,111,128,098,0000,1,0\n\
C1.10,110,128,098,0000,1,0\n\
C1.10,110,128,098,0000,1,0\n\
C1.10,110,128,098,0000,1,0\n\
C1.10,110,128,098,0000,1,0\n\
C1.10,111,128,097,0000,1,0\n\
C1.10,111,128,097,0000,1,0\n\
C1.10,111,128,097,0000,1,0\n\
C1.10,111,128,097,0000,1,0\n\
C1.10,112,128,097,0000,1,0\n\
C1.10,112,128,097,0000,1,0\n\
C1.10,112,128,097,0000,1,0\n\
C1.10,112,128,097,0000,1,0\n\
C1.10,113,128,097,0000,1,0\n\
C1.10,113,128,097,0000,1,0\n\
C1.10,113,128,097,0000,1,0\n\
C1.10,113,128,097,0000,1,0\n\
C1.10,114,128,097,0000,1,0\n\
C1.10,114,128,097,0000,1,0\n\
C1.10,114,128,097,0000,1,0\n\
C1.10,114,128,097,0000,1,0\n\
C1.10,115,128,097,0000,1,0\n\
C1.10,115,128,097,0000,1,0\n\
C1.10,115,128,097,0000,1,0\n\
C1.10,115,128,097,0000,1,0\n\
C1.10,116,128,096,0000,1,0\n\
C1.10,116,128,096,0000,1,0\n\
C1.10,116,128,096,0000,1,0\n\
C1.10,116,128,096,0000,1,0\n\
C1.10,117,128,096,0000,1,0\n\
C1.10,117,128,096,0000,1,0\n\
C1.10,117,128,096,0000,1,0\n\
C1.10,117,128,096,0000,1,0\n\
C1.10,118,128,096,0000,1,0\n\
C1.10,118,128,096,0000,1,0\n\
C1.10,118,128,096,0000,1,0\n\
C1.10,118,128,096,0000,1,0\n\
C1.10,119,128,096,0000,1,0\n\
C1.10,119,128,096,0000,1,0\n\
C1.10,119,128,096,0000,1,0\n\
C1.10,119,128,096,0000,1,0\n\
C1.10,120,128,096,0000,1,0\n\
C1.10,120,128,096,0000,1,0\n\
C1.10,120,128,096,0000,1,0\n\
C1.10,120,128,096,0000,1,0\n\
C1.10,121,128,096,0000,1,0\n\
C1.10,121,128,096,0000,1,0\n\
C1.10,121,128,096,0000,1,0\n\
C1.10,121,128,096,0000,1,0\n\
C1.10,122,128,096,0000,1,0\n\
C1.10,122,128,096,0000,1,0\n\
C1.10,122,128,096,0000,1,0\n\
C1.10,122,128,096,0000,1,0\n\
C1.10,123,128,096,0000,1,0\n\
C1.10,123,128,096,0000,1,0\n\
C1.10,123,128,096,0000,1,0\n\
C1.10,123,128,096,0000,1,0\n\
C1.10,124,128,096,0000,1,0\n\
C1.10,124,128,096,0000,1,0\n\
C1.10,124,128,096,0000,1,0\n\
C1.10,124,128,096,0000,1,0\n\
C1.10,125,128,096,0000,1,0\n\
C1.10,125,128,096,0000,1,0\n\
C1.10,125,128,096,0000,1,0\n\
C1.10,125,128,096,0000,1,0\n\
C1.10,126,128,096,0000,1,0\n\
C1.10,126,128,096,0000,1,0\n\
C1.10,126,128,096,0000,1,0\n\
C1.10,126,128,096,0000,1,0\n\
C1.10,127,128,096,0000,1,0\n\
C1.10,127,128,096,0000,1,0\n\
C1.10,127,128,096,0000,1,0\n\
C1.10,127,128,096,0000,1,0\n\
C1.10,128,136,096,0000,1,1\n\
C1.10,128,136,096,0000,1,1\n\
C1.10,128,136,096,0000,1,1\n\
C1.10,128,136,096,0000,1,1\n\
C1.10,128,136,097,0000,1,1\n\
C1.10,128,136,097,0000,1,1\n\
C1.10,128,136,098,0000,1,1\n\
C1.10,129,136,099,0000,1,1\n\
C1.10,129,136,099,0000,1,1\n\
C1.10,129,136,100,0000,1,1\n\
C1.10,129,136,101,0000,1,1\n\
C1.10,129,136,102,0000,1,1\n\
C1.10,129,136,103,0000,1,1\n\
C1.10,129,136,103,0000,1,1\n\
C1.10,129,136,104,0000,1,1\n\
C1.10,129,136,105,0000,1,1\n\
C1.10,129,136,105,0000,1,1\n\
C1.10,129,136,106,0000,1,1\n\
C1.10,129,136,106,0000,1,1\n\
C1.10,129,136,107,0000,1,1\n\
C1.10,129,136,107,0000,1,1\n\
C1.10,129,136,107,0000,1,1\n\
C1.10,129,136,107,0000,1,1\n\
C1.10,129,136,107,0000,1,1\n\
C1.10,130,136,107,0000,1,1\n\
C1.10,130,136,107,0000,1,1\n\
C1.10,130,136,107,0000,1,1\n\
C1.10,130,136,106,0000,1,1\n\
C1.10,130,136,106,0000,1,1\n\
C1.10,130,136,106,0000,1,1\n\
C1.10,130,136,106,0000,1,1\n\
C1.10,130,136,105,0000,1,1\n\
C1.10,130,136,105,0000,1,1\n\
C1.10,130,136,105,0000,1,1\n\
C1.10,130,136,104,0000,1,1\n\
C1.10,130,136,104,0000,1,1\n\
C1.10,130,136,104,0000,1,1\n\
C1.10,130,136,104,0000,1,1\n\
C1.10,130,136,103,0000,1,1\n\
C1.10,130,136,103,0000,1,1\n\
C1.10,130,136,103,0000,1,1\n\
C1.10,131,136,102,0000,1,1\n\
C1.10,131,136,102,0000,1,1\n\
C1.10,131,136,102,0000,1,1\n\
C1.10,131,136,101,0000,1,1\n\
C1.10,131,136,101,0000,1,1\n\
C1.10,131,136,100,0000,1,1\n\
C1.10,131,136,100,0000,1,1\n\
C1.10,131,136,100,0000,1,1\n\
C1.10,131,136,100,0000,1,1\n\
C1.10,131,136,099,0000,1,1\n\
C1.10,131,136,099,0000,1,1\n\
C1.10,131,136,099,0000,1,1\n\
C1.10,131,126,099,0000,1,0\n\
C1.10,132,126,099,0000,1,0\n\
C1.10,132,126,099,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,132,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,133,126,098,0000,1,0\n\
C1.10,134,126,098,0000,1,0\n\
C1.10,134,126,098,0000,1,0\n\
C1.10,134,126,098,0000,1,0\n\
C1.10,134,126,098,0000,1,0\n\
C1.10,134,126,098,0000,1,0\n\
C1.10,134,126,098,0000,1,0\n\
";