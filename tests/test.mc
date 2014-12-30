double x, n, res;
double c, v, b;
{
    res = 1.e+32;
	read(x, n); 
	while [n != 0] do
	{
		while [c > b || c < v] do
		{
	    	res = res * x;
			write("hello");
		};
		n = n - 1;
	};
	if n == res && v != b then
	{
		read(res);
	}
	else
	{
		write(v, b, "test string");
	};
}
