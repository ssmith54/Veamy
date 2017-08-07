%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%                            PolyMesher2Veamy
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%                                   
%            Version      : 1.0                         
%            Date         : August 3, 2017  
%            Source code  : http://camlab.cl/research/software/veamy/
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%            Created by   : A. Ortiz-Bernardin 
%            E-mail       : aortizb@uchile.cl  
%            Homepage     : http://camlab.cl/alejandro
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%                          Copyright and License                               
%                          =====================                                                                                
%                           Copyright (c) 2017                     
%                     by Alejandro Ortiz-Bernardin                                          
%                 Department of Mechanical Engineering                        
%                 University of Chile, Santiago, CHILE                        
%                          All Rights Reserved.                                
%                                                                              
%   This program is free software: you can redistribute it and/or modify
%   it under the terms of the GNU General Public License as published by
%   the Free Software Foundation, either version 3 of the License, or
%   (at your option) any later version.
%
%   This program is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with this program.  If not, see <http://www.gnu.org/licenses/>.                                         
%                                                                              
%-------------------------------------------------------------------------------
% Release notes 
% =============
% Version 1.0 (August 3, 2017): initial release of the code.
%
%-------------------------------------------------------------------------------
% References 
% ==========
% This code has been used for PolyMesher to Veamy mesh translation in Ref. [1].
%
% [1] A. Ortiz-Bernardin, C. Alvarez, N. Hitschfeld-Kahler, A. Russo, 
%     R. Silva, A. Olate-Sanzana, "Veamy: an extensible object-oriented 
%     C++ library for the virtual element method," XX-XX-XX-XX 2017; 
%     XX(XX):XX-XX. http://camlab.cl/research/software/veamy
%
% [2] C Talischi, GH Paulino, A Pereira, IFM Menezes, 
%     "PolyMesher: A general-purpose mesh generator for polygonal elements 
%     written in Matlab", Struct Multidisc Optim, 2012,
%     DOI 10.1007/s00158-011-0706-z                                      
%
%-------------------------------------------------------------------------------
% Purpose
% =======
% Read a PolyMesher [2] mesh and write it into a Veamy mesh format.
%
% Usage
% =====
% PolyMesher2Veamy(Node,Element,NElem,Supp,Load)
%
% Input
% =====
% Node    : name of the file containing the polygonal mesh (string)
% Element : name of the file containing the nodal displacements (string)
% NElem   : title for the plot of nodal results in the X direction
% Supp    : title for the plot of nodal results in the Y direction
% Load    : title for the plot of the norm of nodal results
%
% Output
% ======
% A text file 'polymesher2veamy.txt' containing a PolyMesh [2] polygonal mesh 
% along with boundary conditions that is readable into Veamy [1].
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function PolyMesher2Veamy(Node,Element,NElem,Supp,Load)
fprintf('Printing mesh to a Veamy mesh format\n');
fid = fopen('polymesher2veamy.txt','wt');                      
NNode = size(Node,1);
fprintf(fid,'%d\n',NNode);                                    %Nodal coordinates
for node_i = 1:NNode
  fprintf(fid,'%.8f %.8f\n', Node(node_i,1), Node(node_i,2));  
end
fprintf(fid,'%d\n',NElem);                                 %Element connectivity
for el = 1:NElem
  NVertex = length(Element{el});
  fprintf(fid,'%d ', NVertex);
  for vertex = 1:(NVertex-1)
    fprintf(fid,'%d ', Element{el}(vertex));
  end
  fprintf(fid,'%d\n', Element{el}(NVertex));
end
NFix = size(Supp,1);                   %Nodal constraints ( 0 = fixed, 1 = free)
fprintf(fid,'%d\n',NFix);
for fixnode = 1:NFix
  fprintf(fid,'%d %d %d\n', Supp(fixnode,1), Supp(fixnode,2),...
                                      Supp(fixnode,3));
end
NLoad = size(Load,1);                               %Neumann boundary conditions
fprintf(fid,'%d\n',NLoad);
for loadednode = 1:NLoad
  fprintf(fid,'%d %.8f %.8f\n', Load(loadednode,1), Load(loadednode,2),...
                                      Load(loadednode,3));
end
