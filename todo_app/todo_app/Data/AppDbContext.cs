using Microsoft.EntityFrameworkCore;
using todo_app.Models;

namespace todo_app.Data
{
    public class AppDbContext : DbContext
    {
        public AppDbContext(DbContextOptions<AppDbContext> options) : base(options) { }

        public DbSet<TodoItem> TodoItems { get; set; }
    }
}
