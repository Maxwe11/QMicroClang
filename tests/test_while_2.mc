double n, t;
{
	while n <= 3 do
	{
		t = 0;
		while t < 6 do
		{
	    	write("[", n, ", ", t, "]");
			t = t + 1;
		};
		n = n + 1;
	};
}
