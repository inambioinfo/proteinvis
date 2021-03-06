#version 330 compatibility
#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable

//#define ENABLE_TIPS

#ifndef ENABLE_TIPS
layout( lines_adjacency ) in;
layout( triangle_strip, max_vertices=16 ) out;
#else
layout( triangles ) in;
layout( triangle_strip, max_vertices=24 ) out;
#endif

out vec3 normal;
out vec3 wc_pos;

uniform vec3 g_helixUp;

const float width  = 1.2f;
const float height = 0.4;

void draw_quad(vec3 a, vec3 b,vec3 c,vec3 d)
{
  wc_pos      = (gl_ModelViewMatrix*vec4(a,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  wc_pos = (gl_ModelViewMatrix*vec4(b,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  wc_pos = (gl_ModelViewMatrix*vec4(c,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  wc_pos = (gl_ModelViewMatrix*vec4(d,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  EndPrimitive();
}

void draw_quad(vec3 a,vec3 an, vec3 b,vec3 bn,vec3 c,vec3 cn,vec3 d,vec3 dn)
{
  normal      = an;
  wc_pos      = (gl_ModelViewMatrix*vec4(a,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  normal      = bn;
  wc_pos = (gl_ModelViewMatrix*vec4(b,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  normal      = cn;
  wc_pos = (gl_ModelViewMatrix*vec4(c,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  normal      = dn;
  wc_pos = (gl_ModelViewMatrix*vec4(d,1)).xyz; 
  gl_Position = gl_ProjectionMatrix * vec4(wc_pos,1); EmitVertex();

  EndPrimitive();
}

void main()
{
#ifndef ENABLE_TIPS
  vec3 p = gl_PositionIn[1].xyz;
  vec3 q = gl_PositionIn[2].xyz;
  vec3 r = gl_PositionIn[3].xyz;
#else
  vec3 p = gl_PositionIn[0].xyz;
  vec3 q = gl_PositionIn[1].xyz;
  vec3 r = gl_PositionIn[2].xyz;
#endif

  vec3 up    = g_helixUp;
  vec3 pq    = q-p;
  vec3 qr    = r-q;
  
#ifdef ENABLE_TIPS
  if(length(qr) < 0.0001)
    qr = pq;
#endif 

  
  vec3 pqxup = normalize(cross(pq,up));
  vec3 qrxup = normalize(cross(qr,up));

  up    *= width;
  pqxup *= height;
  qrxup *= height;

  vec3 pup_wc    = normalize(gl_NormalMatrix*(cross(pqxup,pq)));
  vec3 qup_wc    = normalize(gl_NormalMatrix*(cross(qrxup,pq)));
  vec3 pqxup_wc  = normalize(gl_NormalMatrix*pqxup);
  vec3 qrxup_wc  = normalize(gl_NormalMatrix*qrxup);

  gl_FrontColor = vec4(gl_FrontColorIn[1].xyz*0.5,1);


  draw_quad(p+up-pqxup,pup_wc,
            p+up+pqxup,pup_wc,
            q+up-qrxup,qup_wc,
            q+up+qrxup,qup_wc);

  draw_quad(p-up-pqxup,-pup_wc,
            q-up-qrxup,-qup_wc,
            p-up+pqxup,-pup_wc,
            q-up+qrxup,-qup_wc);

  gl_FrontColor = gl_FrontColorIn[1];

  draw_quad(p+up+pqxup,pqxup_wc,
            p-up+pqxup,pqxup_wc,
            q+up+qrxup,qrxup_wc,
            q-up+qrxup,qrxup_wc);

  draw_quad(p+up-pqxup,-pqxup_wc,
            q+up-qrxup,-qrxup_wc,
            p-up-pqxup,-pqxup_wc,
            q-up-qrxup,-qrxup_wc);

#ifdef ENABLE_TIPS

  gl_FrontColor = vec4(gl_FrontColorIn[1].xyz*0.5,1);
  
  normal = normalize(gl_NormalMatrix*(-pq));

  draw_quad(p+up+pqxup,
            p+up-pqxup,
            p-up+pqxup,
            p-up-pqxup);

  normal = normalize(gl_NormalMatrix*(qr));

  draw_quad(q+up+qrxup,
            q-up+qrxup,
            q+up-qrxup,
            q-up-qrxup);

#endif

}
