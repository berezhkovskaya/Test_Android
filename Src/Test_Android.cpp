#include "Linderdaum.h"

sEnvironment* Env = NULL;

LMatrix4              Projection;
clVirtualTrackball    Trackball;
clGameCamera*         Camera = NULL;
clScene*              Scene  = NULL;

void DrawOverlay(LEvent Event, const LEventArgs& Args)
{
    // ������� ������
    LMatrix4 Trans( Trackball.GetRotationMatrix() * Camera->GetCamera().GetModelViewMatrix() );
    Scene->SetCameraTransform( Trans );
    Scene->SetCameraProjection( Projection );

    // �������� �����
    Scene->SetUseOffscreenBuffer( false, false );
    Scene->RenderForward();

    // ������� ����������� �������
    bool MousePressedL = Env->Console->IsKeyPressed( LK_LBUTTON );

    Env->Viewport->UpdateTrackball( &Trackball, 10.0f, MousePressedL );
}

void Update( LEvent Event, const LEventArgs& Args )
{
    // � Args.FFloatArg �������� DeltaTime � �������������
}

APPLICATION_ENTRY_POINT
{
    LString CommandLine;

    EXTRACT_COMMAND_LINE(CommandLine);

    Env = new sEnvironment();

    // CommonMedia ������������ ������ �� PC, �� �������� ��� ����� ����� ��������� � �������
    Env->DeployDefaultEnvironment( CommandLine, "..\\..\\CommonMedia" );
    Env->FileSystem->Mount("GameData");

    Projection = Math::Perspective( 45.0f, Env->Viewport->GetAspectRatio(), 0.4f, 2000.0f );

    // �������� ������ � �������� � �� �������
    Camera = Env->Linker->Instantiate( "clGameCamera" );
    Camera->GetCamera().SetPosition( LVector3(0,-10,10) );
    CONNECTOBJ( L_EVENT_TIMER, &clGameCamera::Event_TIMER, Camera );

    Env->Connect( L_EVENT_DRAWOVERLAY, Utils::Bind( &DrawOverlay ) );
    Env->Connect( L_EVENT_TIMER,       Utils::Bind( &Update      ) );

    // �������� �����
    Scene = Env->Linker->Instantiate("clScene");

    // � ������� � �� ���
    int ID = Scene->AddGeom( Env->Resources->CreateIcosahedron( 3.0f, LVector3(0) ) );

    // ��������� ��� ��������
    clMaterial* Mtl = Env->Resources->CreateMaterial();
    Mtl->SetPropertyValue( "DiffuseColor", "1.0 0.0 0.0 0" );
    Mtl->SetPropertyValue( "CastShadow",   "false" );
    Scene->SetMtl( ID, Mtl );

    // ��������� ���������
    Scene->SetLocalTransform( ID, LMatrix4::GetTranslateMatrix( LVector3(  0.0f, 0.0f, 0.0f ) ) );

    Env->RunApplication( DEFAULT_CONSOLE_AUTOEXEC );

    APPLICATION_EXIT_POINT( Env );
}

APPLICATION_SHUTDOWN
{
}