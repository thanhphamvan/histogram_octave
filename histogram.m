function res = histogram(mat)
	res = sum(hist_omp(mat), 1)
end
