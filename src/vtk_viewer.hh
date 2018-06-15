#pragma once

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkXMLStructuredGridWriter.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkLookupTable.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkPoints.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkXMLPolyDataWriter.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

namespace vtk
{
  class VtkViewer
  {
    public:
      VtkViewer();
      void init(int, int, int);
      void update(const std::vector<double>&, int, int, int, double);
      void show();
  
    private:
      vtkSmartPointer<vtkStructuredGrid> structured_grid_;
      vtkSmartPointer<vtkPoints> points_;
      vtkSmartPointer<vtkUnsignedCharArray> colors_;
      vtkSmartPointer<vtkStructuredGridGeometryFilter> geometry_filter_;
      vtkSmartPointer<vtkPolyDataMapper> mapper_;
      vtkSmartPointer<vtkActor> actor_;
      vtkSmartPointer<vtkRenderer> renderer_;
      vtkSmartPointer<vtkRenderWindow> render_window_;
      vtkSmartPointer<vtkRenderWindowInteractor> render_window_interactor_;
  };
}
