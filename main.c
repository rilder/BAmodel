#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mtwister.h"
#include "mtwister.c"

/**
 * @file main.c
 * @author Rilder Pires
 * @version 1.0
 *
 * @section LICENSE
 *
 * Barabasi Albert Algorithm
 * Copyright (C) 2013  Rilder Pires
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @section DESCRIPTION
 * 
 * This is an C implementation of Barabasi Albert algorithm coded by Rilder
 * Pires in 2013/08/30. The original idea of this algorithm was proposed by
 * Barabasi and Albert in 1999. It was decribed into a paper called
 * "Emergence of Scaling in Random Networks".
 *
 * Any feedback is very welcome.
 * e-mail: rilder@fisica.ufc.br    
 */

int main(){
	int i,j,ii,cv,ok;
	double r;
	int m0=5;
	int m=5;
	int t=150000;
	
	int *edges;
	int *k;
	int *dist;
	int kmax=0;
	
	/* Set the seed of Merssene Twister */
	MTwister_Init(764763469);
	
	/* Alloc memory to keep  */
	edges=(int*)calloc(m*t,sizeof(int)); 
	k=(int*)calloc(m0+t,sizeof(int));

	/* Edges of the network */
	/* edges[i*m+j] (0 <= j < m) are the j vertices that are linked to i. */ 
	for(j=0;j<m*t;j++) edges[j]=-1;
	
	/* This statement deal with the inclusion of the first vertex of the network */
	for(j=0;j<m;j++){
		edges[j]=j;
		k[j]++;
		k[m0+0]++;
	};
	kmax=k[m0+0];

	/* This statement deal with the inclusion of the other vertices of the network */
	for(i=1;i<t;i++){
		/* Put m edges to the i vertex. */
		for(ii=0;ii<m;ii++){
			ok=0;
			/* Sort a vertex until it satisfy our conditions (Regection Method).*/
			while(ok==0){
				cv=MTwister_Int31()%(m0+i);
				r=MTwister_Real3()*((double)kmax);
				/* See if the choosen point is below the probability curve. */
				if(r<((double)k[cv])){
					ok=1;
					for(j=0;j<ii;j++){
						if(edges[i*m+j]==cv) ok=0;
					};
				};
			};
			/* The vertex was chosen, so now is time to include it to our list. */
			edges[i*m+ii]=cv;
			k[cv]++;
			k[m0+i]++;
			/* Here we store the value of the maximum of the probability distribution. */
			if(k[cv]>kmax) kmax=k[cv];
			if(k[m0+i]>kmax) kmax=k[m0+i];
		};
	};

	/* Deal with statistics stuf (in the future it's a good choice make this in
	other program) */
	dist=(int*)calloc(kmax+1,sizeof(int));
	
	ii=0;
	for(i=0;i<m0+t;i++){
		dist[k[i]]++;
		ii+=k[i];
		if(k[i]==-1) printf("%d\n",k[i]);
	};
	
	j=0;
	for(i=0;i<kmax+1;i++){
		printf("%d %d\n",i,dist[i]);
		j+=dist[i];
	};

	free(edges);
	free(dist);
	free(k);
	return 0;
}
