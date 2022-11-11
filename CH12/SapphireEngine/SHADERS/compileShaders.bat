@echo off
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\bypass_ps.cso" /T"ps_6_0" /nologo ../bypass_ps.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\bypass_vs.cso" /T"vs_6_0" /nologo ../bypass_vs.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\directional_ps.cso" /T"ps_6_0" /nologo ../directional_ps.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\directional_ps.cso" /T"vs_6_0" /nologo ../directional_vs.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\directional_texture_ps.cso" /T"ps_5_0" /nologo -D USE_BUMP_MAP ../directional_texture_ps.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\directional_texture_vs.cso" /T"vs_5_0" /nologo -D USE_BUMP_MAP ../directional_texture_vs.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\directional_texture_ps_1.cso" /T"ps_5_0" /nologo ../directional_texture_ps.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\directional_texture_vs_1.cso" /T"vs_5_0" /nologo ../directional_texture_vs.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\shadow_ps.cso" /T"ps_5_0" /nologo ../shadow_ps.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\shadow_vs.cso" /T"vs_5_0" /nologo ../shadow_vs.hlsl
dxc.exe /E"main" /Fo"C:\Users\boczu\source\repos\GameEngineCourse\CH12\x64\Release\fullscreen_vs.cso" /T"vs_5_0" /nologo ../fullscreen_vs.hlsl