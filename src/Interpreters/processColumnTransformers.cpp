#include <Interpreters/processColumnTransformers.h>

#include <Interpreters/DatabaseAndTableWithAlias.h>
#include <Interpreters/TranslateQualifiedNamesVisitor.h>
#include <Interpreters/getTableExpressions.h>
#include <Parsers/ASTIdentifier.h>
#include <Parsers/ASTTablesInSelectQuery.h>
#include <Parsers/IAST.h>
#include <Storages/IStorage.h>
#include <Storages/StorageInMemoryMetadata.h>

namespace DB
{

namespace
{
ASTPtr processColumnTransformersImpl(
    const ColumnsDescription & columns,
    const NamesAndTypesList & virtual_columns,
    ASTPtr query_columns,
    const String & current_database,
    const StorageID & storage_id)
{
    auto names_and_types = columns.getOrdinary();
    removeDuplicateColumns(names_and_types);

    TablesWithColumns tables_with_columns;
    {
        auto table_expr = std::make_shared<ASTTableExpression>();
        table_expr->database_and_table_name = std::make_shared<ASTTableIdentifier>(storage_id);
        table_expr->children.push_back(table_expr->database_and_table_name);
        tables_with_columns.emplace_back(DatabaseAndTableWithAlias(*table_expr, current_database), names_and_types);
    }

    tables_with_columns[0].addHiddenColumns(columns.getMaterialized());
    tables_with_columns[0].addHiddenColumns(columns.getAliases());
    tables_with_columns[0].addHiddenColumns(virtual_columns);

    NameSet source_columns_set;
    for (const auto & identifier : query_columns->children)
        source_columns_set.insert(identifier->getColumnName());

    TranslateQualifiedNamesVisitor::Data visitor_data(source_columns_set, tables_with_columns);
    TranslateQualifiedNamesVisitor visitor(visitor_data);
    auto columns_ast = query_columns->clone();
    visitor.visit(columns_ast);

    return columns_ast;
}
}

ASTPtr processColumnTransformers(
        const String & current_database,
        const StoragePtr & table,
        const StorageMetadataPtr & metadata_snapshot,
        ASTPtr query_columns)
{
    return processColumnTransformersImpl(metadata_snapshot->columns, table->getVirtualsList(), query_columns, current_database, table->getStorageID());
}

ASTPtr processColumnTransformers(
        const String & current_database,
        const StorageID & table_id,
        const ColumnsDescription & columns,
        ASTPtr query_columns)
{
    return processColumnTransformersImpl(columns, {}, query_columns, current_database, table_id);
}

}
