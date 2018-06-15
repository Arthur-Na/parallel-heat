#include "vtk_viewer.hh"

namespace vtk
{
  VtkViewer::VtkViewer()
    : structured_grid_(vtkSmartPointer<vtkStructuredGrid>::New())
    , points_(vtkSmartPointer<vtkPoints>::New())
    , colors_(vtkSmartPointer<vtkUnsignedCharArray>::New())
    , geometry_filter_(vtkSmartPointer<vtkStructuredGridGeometryFilter>::New())
    , mapper_(vtkSmartPointer<vtkPolyDataMapper>::New())
    , actor_(vtkSmartPointer<vtkActor>::New())
    , renderer_(vtkSmartPointer<vtkRenderer>::New())
    , render_window_(vtkSmartPointer<vtkRenderWindow>::New())
    , render_window_interactor_(vtkSmartPointer<vtkRenderWindowInteractor>::New())
  {}

  void VtkViewer::init(int size_x, int size_y, int size_z)
  {
    for (int i = 0; i < size_x; ++i)
      for (int j = 0; j < size_y; ++j)
        for (int k = 0; k < size_z; ++k)
          points_->InsertNextPoint(i, j, k);

    structured_grid_->SetDimensions(size_x, size_y, size_z);
    structured_grid_->SetPoints(points_);

    colors_->SetNumberOfComponents(3);
    colors_->SetName("colors");

    mapper_->SetInputConnection(geometry_filter_->GetOutputPort());

    actor_->SetMapper(mapper_);
    actor_->GetProperty()->SetPointSize(3);

    structured_grid_->GetPointData()->SetScalars(colors_);

    render_window_->AddRenderer(renderer_);
    render_window_interactor_->SetRenderWindow(render_window_);

    renderer_->AddActor(actor_);
    renderer_->SetBackground(.0, .0, .0);

    render_window_->Render();
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    //render_window_interactor->Start();
    render_window_interactor_->Initialize();
  }

  void VtkViewer::update(const std::vector<double>& vect, int size_x, int size_y, int size_z,
                         double max_val)
  {
    std::cout << "iteration" << std::endl;
    for (unsigned int i = 0; i < structured_grid_->GetNumberOfPoints(); ++i)
    {
      double p[3];
      structured_grid_->GetPoint(i, p);

      //int index = (p[2] * size_x * size_y) + (p[1] * size_x) + p[0];
      int index = (p[0] * size_x + p[1]) * size_y + p[2];
          //new_mesh.at((i * x_ + j) * y_ + k) = compute(i, j, k);
      double val = vect[index];

      float coeff = val / max_val;
      unsigned char red = static_cast<unsigned char>(255 * coeff);
      unsigned char blue = static_cast<unsigned char>(255 * (1 - coeff));
      unsigned char color[3] = { red, 0, blue };

#if VTK_MAJOR_VERSION > 7
      colors_->InsertNextTypedTuple(color);
#else
      colors_->InsertNextTupleValue(color);
#endif
    }

    structured_grid_->GetPointData()->SetScalars(colors_);
    structured_grid_->Modified();
#if VTK_MAJOR_VERSION <= 5
    geometry_filter_->SetInputConnection(structured_grid_->GetProducerPort());
#else
    geometry_filter_->SetInputData(structured_grid_);
#endif
    geometry_filter_->Modified();
    geometry_filter_->Update();
    mapper_->Modified();
    mapper_->Update();
    render_window_->Render();
  }

  void VtkViewer::show()
  {
    std::cout << "end" << std::endl;
    render_window_interactor_->Start();
  }
}
