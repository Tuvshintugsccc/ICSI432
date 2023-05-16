[11:50 AM] AMARTUGS TEMUULEN




for (int i = 0; i < 100001; i++){

        GLfloat v1x = p[f[i].b].x - p[f[i].a].x;

        GLfloat v1y = p[f[i].b].y - p[f[i].a].y;

        GLfloat v1z = p[f[i].b].z - p[f[i].a].z;

        GLfloat v2x = p[f[i].c].x - p[f[i].a].x;

        GLfloat v2y = p[f[i].c].y - p[f[i].a].y;

        GLfloat v2z = p[f[i].c].z - p[f[i].a].z;

        GLfloat nx = v1y * v2z - v1z * v2y;

        GLfloat ny = v1z * v2x - v1x * v2z;

        GLfloat nz = v1x * v2y - v1y * v2x;

        GLfloat normalLength = sqrt(nx * nx + ny * ny + nz * nz);

        if (normalLength > 0.0) {

            nx /= normalLength;

            ny /= normalLength;

            nz /= normalLength;

        }

        GLfloat normal[] = {nx, ny, nz};

        glNormal3fv(normal);

 

    glBegin(GL_TRIANGLES);

        glVertex3f( p [f[i].a].x, p [f[i].a].y, p [f[i].a].z );

        glVertex3f( p [f[i].b].x, p [f[i].b].y, p [f[i].b].z );

        glVertex3f( p [f[i].c].x, p [f[i].c].y, p [f[i].c].z );

    }


